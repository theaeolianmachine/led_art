#include "font_brando.h"
#include "message_lexicon.h"
#include <FastLED.h>
#include <antplusdefs.h>
#include <USBHost_t36.h>
#include <SD.h>
#include "src/animatedgifs/GifDecoder.h"
#include "src/animatedgifs/FilenameFunctions.h"


/*
 *  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 to 220 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4:  Do not use
    pin 3:  SD Card, CS
    pin 11: SD Card, MOSI
    pin 12: SD Card, MISO
    pin 13: SD Card, SCLK
*/

// Chip select for SD card on the SmartMatrix Shield or Photon
#if defined(ESP32)
    #define SD_CS 5
#elif defined (ARDUINO)
    // #define SD_CS 15
    #define SD_CS BUILTIN_SDCARD
#elif defined (SPARK)
    #define SD_CS SS
#endif

#if defined(ESP32)
    // ESP32 SD Library can't handle a trailing slash in the directory name
    #define GIF_DIRECTORY "/gifs"
#else
    // Teensy SD Library requires a trailing slash in the directory name
    #define GIF_DIRECTORY "/gifs/"
#endif
#define DISPLAY_TIME_SECONDS 30

/* SmartMatrix configuration and memory allocation */
#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 32;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 2;       // known working: 2-4
const uint8_t gifXOffset = 2;
const uint8_t gifYOffset = 2;

int num_files;
int gifIndex = 88;

GifDecoder<kMatrixWidth, kMatrixHeight, 12> decoder;

#define NUM_LEDS 150 * 9
#define DATA_PIN 3

CRGB leds[NUM_LEDS];
CRGB colorAt[NUM_LEDS];

USBHost myhost;
USBHub hub(myhost);
KeyboardController keycon(myhost);

uint32_t frame_counter = 0;
uint32_t last_key_press_frame = 0;
uint32_t last_phrase_frame = 0;

#define NUM_COLUMNS 36
#define NUM_ROWS 37

// num leds for a letter (plus gap)
#define LETTER_WIDTH 4
#define LETTER_HEIGHT 6
#define BOARD_WIDTH 36
#define BOARD_HEIGHT 35
#define CURSOR_BLINK_RATE 500
#define COLOR_CHANGE_RATE 100
#define MAX_CHARS 54
#define FRAME_HZ 27
#define BRIGHTNESS 150

#define USING_BRANDO_FONT true
#define DEBUGGING false

CRGB colors[MAX_CHARS];
bool board_is_full = false; // sigh we only know board is full in loop() but we want to let keypress() also know

char msg[MAX_CHARS];
int curr_char_index = 0;

#define PHRASE_DURATION 10
#define GIF_DURATION 30
#define IDLE_TIME_UNTIL_EXIT_EDIT_MODE 30  // if no keystrokes after 60s, exit edit mode

enum appstate {
  TEXT,
  GIF,
  QUOTE
};

appstate state = QUOTE;
uint32_t last_state_transition = 0;

enum readquotestate {
  READ_PHRASE,
  END_PHRASE,
};

struct Phrase {
  String msg;
  Phrase* next_phrase = 0;
};

struct Quote {
  Phrase* head_phrase = 0;
  Quote* next_quote = 0;
};

Quote head_quote;
Quote* choosen_quote = 0;
Phrase* choosen_phrase = 0;
int num_quotes = 0;
int quote_index = -1;


void setup() {
  Serial.begin(9600);//setup serial to debug
  delay(1000); // delay helps with debug

  strcpy(msg, "");
  
  myhost.begin();
  keycon.attachPress(keypress);
  randomSeed(analogRead(0));

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  decoder.setScreenClearCallback(screenClearCallback);
  decoder.setUpdateScreenCallback(updateScreenCallback);
  decoder.setDrawPixelCallback(drawPixelCallback);

  decoder.setFileSeekCallback(fileSeekCallback);
  decoder.setFilePositionCallback(filePositionCallback);
  decoder.setFileReadCallback(fileReadCallback);
  decoder.setFileReadBlockCallback(fileReadBlockCallback);

  // UNCOMMENT THIS ONCE YOU HAVE AN SD CARD
  initSdCard(SD_CS);
  num_files = enumerateGIFFiles("/gifs/", false);
  gifIndex = random(1,num_files);

  loadQuotes();
  // printQuotes();
}//==end of setup==//

void loadQuotes() {
  File quotes_file = SD.open("quotes.txt");
  char reading_buffer[MAX_CHARS+1];
  int buffer_idx = 0;
  bool skipped_phrase = false;
  readquotestate rqs = READ_PHRASE;
  Quote* current_quote = &head_quote;
  while (quotes_file.available() > 0) {
    int chr = quotes_file.read();
    if (chr == -1) {
      break;
    }
    switch(rqs) {
      case READ_PHRASE: {
        if (chr == '\n') {
          // skip leading new lines
          if (buffer_idx != 0) {
            rqs = END_PHRASE;
          }
        } else {
          if (buffer_idx < MAX_CHARS) {
            reading_buffer[buffer_idx++] = chr;
          } else {
            skipped_phrase = true;
          }
        }
        break;
      }
      case END_PHRASE: {
        reading_buffer[buffer_idx] = '\0';
        buffer_idx = 0;
        if (skipped_phrase) {
          Serial.print("Warning, truncated phrase: ");
          Serial.println(reading_buffer);
        }

        skipped_phrase = false;

        Phrase* new_phrase = new Phrase;
        new_phrase->msg = reading_buffer;
        append_phrase_to_quote(current_quote, new_phrase);
        if (chr == '\n') {
          Quote* new_quote = new Quote;
          current_quote->next_quote = new_quote;
          current_quote = new_quote;
        } else {
          reading_buffer[buffer_idx++] = chr;
        }
        rqs = READ_PHRASE;
        break;
      }
    }
  }
  // write whatever remains to one last phrase
  reading_buffer[buffer_idx] = '\0';
  Phrase* new_phrase = new Phrase;
  new_phrase->msg = reading_buffer;
  append_phrase_to_quote(current_quote, new_phrase);

  Quote* quote = &head_quote;
  while (quote) {
    num_quotes += 1;
    quote = quote->next_quote;
  }
}

void printQuotes() {
  Serial.println("PRINTING QUOTES");
  Quote* current_quote = &head_quote;
  while (current_quote) {
    Phrase* phrase = current_quote->head_phrase;
    Serial.println("BEGIN QUOTE");
    while (phrase) {
      Serial.println(phrase->msg);
      phrase = phrase->next_phrase;
    }
    Serial.println("END QUOTE");
    current_quote = current_quote->next_quote;
  }
}

void append_phrase_to_quote(Quote* current_quote, Phrase* phrase) {
  if (current_quote->head_phrase == 0) {
    current_quote->head_phrase = phrase;
    return;
  } else {
    Phrase* last_phrase = current_quote->head_phrase;
    while (last_phrase->next_phrase) {
      last_phrase = last_phrase->next_phrase;
    }
    last_phrase->next_phrase = phrase;
  }
}

void loop() {
  frame_counter += 1;
  // Serial.print("state: ");
  // Serial.print(state);
  // Serial.print(" frame_counter: ");
  // Serial.println(frame_counter);
  if (state == QUOTE) {
    fill_gradient(colorAt, NUM_LEDS, CHSV(frame_counter%255, 255, 255), CHSV((frame_counter+50)%255, 255, 255));
    if(DEBUGGING) {
      printText(String("ABCDEFGHIJKLMNOPQRSTUVWXYZ").toLowerCase());
    } else {
      // Serial.print("not in edit mode. frame_counter: ");
      // Serial.println(frame_counter);
      showQuote();
    }
    FastLED.show();
  } else if (state == GIF) {
    playGifAnimation();
    if (frame_counter - last_state_transition > GIF_DURATION * FRAME_HZ) {
      setState(QUOTE);
    }
  } else if (state == TEXT) {
    showUserMessage();
    FastLED.show();
  }
}//==end of loop==//

void playGifAnimation() {
  // static unsigned long futureTime = 0;

  // static int index = 88;
  decoder.decodeFrame();
}

void screenClearCallback(void) {
  if (state == GIF)
    clearBoard();
}

void updateScreenCallback(void) {
  if (state == GIF)
    FastLED.show();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  if (state == GIF)
    setPixel(getPixelAt(x + gifXOffset,y + gifYOffset), CRGB(red, green, blue));
}

void showQuote() {
  if (choosen_quote == 0 || (frame_counter - last_phrase_frame  > PHRASE_DURATION * FRAME_HZ)) {
    if (choosen_quote == 0 || choosen_phrase->next_phrase == 0) {
      quote_index = quote_index + 1 >= num_quotes ? 0 : quote_index + 1; // cycle back to first message
      choosen_quote = &head_quote;
      for (int i = 0; i < quote_index; ++i) {
        choosen_quote = choosen_quote->next_quote;
      }
      choosen_phrase = choosen_quote->head_phrase;
      setState(GIF);
      return;
    } else {
      choosen_phrase = choosen_phrase->next_phrase;
      last_phrase_frame = frame_counter;
    }
    Serial.print("new phrase choosen: ");
    Serial.println(choosen_phrase->msg);
  }
  String messageChoice = choosen_phrase->msg;
  if(USING_BRANDO_FONT) {
    // in brando font uppercase and lowercase are switched. Bonkers!
    messageChoice = messageChoice.toLowerCase();
  }
  printText(messageChoice);
}

void showUserMessage() {
  printText(String(msg));
  if(frame_counter - last_key_press_frame > IDLE_TIME_UNTIL_EXIT_EDIT_MODE * FRAME_HZ) {
    setState(GIF);
    clearUserMessage();
  }
}

void setPixel(int index, CRGB color) {
  if(index < NUM_LEDS) {
    leds[index] = color;
  }
}

// give an x and y between 0->NUM_COLUMNS and 0->NUM_ROWS, returns the index in the pixel array
// for that x,y. Since the led strip starts at the bottom right of the board and ends at the top left,
// we need to do a bit of translation.
int getPixelAt(const int x, const int y) {
  return getPixelIndex(NUM_COLUMNS - x - 1, NUM_ROWS - y - 1, 37);
}


// helper method for the above method. the beginning of the LED strip is actually at the bottom right side of the board. (we flipped the
// board upside down to make accessing the inputs easier). So this method will give you the pixel
// coordinates in the old system. The above method will flip the result we give back here.
int getPixelIndex(const int x, const int y, const int ledsPerCol) {
  int value = x * ledsPerCol;
  value += x % 2 == 0 ? y : (ledsPerCol - y - 1);
  value += (x/ 4)*2; // Strips are 150 pixels long, so every 4th column we need to add 2 leds
  if (value >= 900) {
    value -= 1;
  }

  return value;
}

// this uses the ASCII table, which is what it was created for
int getIndexOfLetter(char letter) {
  int offset = letter - ' ';
  if (offset >= FONT_ARRAY_LENGTH) return -1; // character not defined

  return offset;
}

// wtf is all this code?!?!??!
int printLetter(char letter, int row, int col, CRGB color) {
  int index = getIndexOfLetter(letter);
  int characterWidth = font[index][0];

  bool isOn = 0;
  for(int y = 1; y < FONT_LETTER_NUM_BYTES; y++) {
    for(int x = 0; x < characterWidth; x++) {
      int pixelX = row + x;
      int pixelY = col + y;
      isOn = bitRead(font[index][y], 7-x) == 1;
      int pixelIndex = getPixelAt(pixelX, pixelY);
      if(pixelIndex < NUM_LEDS) {
        color = colorAt[pixelIndex];
        setPixel(pixelIndex, isOn ? color : CRGB::Black);
      } else {
        Serial.print("HOLY SHIT INDEXING OFF THE END OF LED ARRAY: ");
        Serial.println(pixelIndex);
      }
    }
  }

  return characterWidth;
}

int getWordPixelLength(String word) {
  int len = word.length();
  int pixelLength = 0;
  for (int index=0; index<len; index++) {
    char letter = word.charAt(index);
    int fontIndex = getIndexOfLetter(letter);
    pixelLength += font[fontIndex][0];
  }

  return pixelLength;
}

bool shouldChangeColors() {
  // change every 4 seconds
  return frame_counter % COLOR_CHANGE_RATE == 0;
}

int nextTokenEnd(String text, unsigned tokenStart, String delimiters) {
  for(unsigned i = tokenStart; i < text.length(); i++) {
    for(unsigned d = 0; d < delimiters.length(); d++) {
      if(text.charAt(i) == delimiters.charAt(d)) {
        // the trick to returning delimiters as their own tokens: if i == tokenStart here,
        // then we have a delimiter at the beginning of the string, so return tokenStart+1 as token end
        if(i == tokenStart) {
          return tokenStart + 1;
        }
        return i;
      }
    }
  }
  return text.length();
}

void printText(String text) {
  // we change the colors every 4 seconds
  bool change_colors = shouldChangeColors();
  clearBoard();

  int row = 0, col = 0, word_num = 0;
  int wordEnd = 0;
  int textLength = text.length();

  board_is_full = false;
  for(int wordStart = 0; wordStart < textLength; wordStart = wordEnd) {
    if (row >= BOARD_HEIGHT) { // stop printing if past board height
      board_is_full = true;
      return;
    }
    wordEnd = nextTokenEnd(text, wordStart, " \n");
    String word = text.substring(wordStart, wordEnd);
    int characterWidth;
    int wordPixelLength = getWordPixelLength(word);
    if(word.equals(" ")) {
      // if space is off the end, just move to a newline
      if(col + wordPixelLength > BOARD_WIDTH) {
        row += LETTER_HEIGHT;
        col = 0;
      } else {
        // print out a space if the user typed it, BUT don't print a space at beginning of a line,
        // unless user keeps hitting space (ie previous char is a space)
        if(col > 0 || (wordStart > 0 && text.charAt(wordStart-1) == ' ')) {
          characterWidth = printLetter(' ', col, row, 0);
          col += characterWidth;
        }
      }
    } else if(word.equals("\n")) {
      row += LETTER_HEIGHT;
      col = 0;
      if (row >= BOARD_HEIGHT) { // stop printing if past board height
        board_is_full = true;
        return;
      }
    } else {
      // draw all other words normally
      // Break on spaces
      if (col != 0 && (col + wordPixelLength) > BOARD_WIDTH) {
        row += LETTER_HEIGHT;
        col = 0;
        if (row >= BOARD_HEIGHT) { // stop printing if past board height
          board_is_full = true;
          return;
        }
      }

      int wordLength = word.length();
      CRGB color = colors[word_num];
      if(change_colors) {
        color = CRGB(random(1,255),random(1,255),random(1,255));
        // color = CRGB::White;
        colors[word_num] = color;
      }

      for(int index=0; index<wordLength; index++) {
        characterWidth = printLetter(word.charAt(index), col, row, color);
        col += characterWidth;

        if (col + LETTER_WIDTH > BOARD_WIDTH) { // break long word
          row += LETTER_HEIGHT;
          col = 0;
          if (row >= BOARD_HEIGHT) { // stop printing if past board height
            board_is_full = true;
            return;
          }
        }
        word_num++;
      }
    }
  }
  printCursor(row, col, word_num);
}

// if we still have space, we draw "_" as a cursor, and blink it every second.
void printCursor(int row, int col, int word_num) {
  if(state != TEXT) {
    // don't show cursor unless in "edit" mode
    return;
  } else if(col >= BOARD_WIDTH && row >= BOARD_HEIGHT - 1) {
    // no room for cursor
    return;
  } else if(col >= BOARD_WIDTH) {
    row++;
    col = 0;
  }

  // cycle drawing / not drawing cursor every 500ms
  bool show_cursor = (millis() / CURSOR_BLINK_RATE) % 2 == 0;
  if(show_cursor) {
    unsigned long color = colors[word_num];
    if(shouldChangeColors()) {
      color = CRGB(random(1,255),random(1,255),random(1,255));
      colors[word_num] = color;
    }
    printLetter('_', col, row, color);
  }
}

void clearBoard() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void clearUserMessage() {
  msg[0] = '\0';
  curr_char_index = 0;
  board_is_full = false;
}

void setState(appstate new_state) {
  last_state_transition = frame_counter;
  state = new_state;

  switch(state) {
    case GIF:
      if (++gifIndex >= num_files) {
          gifIndex = 0;
      }

      Serial.println("File Index " + String(gifIndex));
      if (openGifFilenameByIndex(GIF_DIRECTORY, gifIndex) >= 0) {
          decoder.startDecoding();
      }
      break;
    case QUOTE:
      break;
    case TEXT:
      break;
    default:
      break;
  }

  last_state_transition = frame_counter;
  last_key_press_frame = frame_counter;
  last_phrase_frame = frame_counter;

  clearBoard();
}

void keypress(int key){
  setState(TEXT);
  last_key_press_frame = frame_counter;
  // SPACEBAR is 32, the first entry in our font list. We only draw ascii codes from space ->
  int offset = key - ' ';
  if(curr_char_index >= MAX_CHARS || curr_char_index < 0) {
    Serial.print("HOLY BALLS curr_char_index > MAX_CHARS!!!!: ");
    Serial.println(curr_char_index);
    return;
  }
  Serial.println(msg);
  if(key == 27) {
    // ESC clear the screen
    clearUserMessage();
  } else if(key == 127) {
    // backspace delete a letter
    if(curr_char_index > 0) {
      curr_char_index--;
      setMsgChar(curr_char_index, '\0');
    }
  } else if(key >= 194 && key <= 205) {
    // F1 through F12, reserved keys. May change mode in future
  } else if(key >= 215 && key <= 218) {
    // RIGHT 215, LEFT 216, DOWN, 217, UP 218
    // Arrow Keys, reserved keys. may use them to play sweeet games in future
  } else if(board_is_full) {
    // if we have a full board of text, don't let the user add any more text.
    Serial.println("board is full. not adding letters");
    return;
  } else if(key == 10) {
    // ENTER is newline
    setMsgChar(curr_char_index, '\n');
    setMsgChar(curr_char_index+1, '\0');
    curr_char_index++;
    curr_char_index %= (MAX_CHARS-1);
  } else if (offset >= 0 && offset < FONT_ARRAY_LENGTH) {
    // other letters just add
    Serial.print("adding letter: ");
    Serial.println((char)key);
    setMsgChar(curr_char_index, (char)key);
    setMsgChar(curr_char_index+1, '\0');
    curr_char_index++;
    curr_char_index %= (MAX_CHARS-1);
  }
}

void setMsgChar(int index, char character) {
  if (index >= 0 && index < MAX_CHARS) {
    msg[index] = character;
  }
}
