
#include <gamma.h>
#include <RGBmatrixPanel.h>

//colors for rgb
#define CLK  11 
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

int scores[3];
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int noteEZ = 50;
int noteMid = 75;
int noteHard = 100;
int GameArry = 0;
int tick = 0;
int score = 0;
int cycles = 0;
int teimer = 0;
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

class Arrow{
    private:
        int y;
        bool moving = false;
        char direction;
    public:
        Arrow(){}

    Arrow(int h, char d, bool isMoving){
        y = h;
        direction = d;
        moving = isMoving;
    }

    Arrow(char d){
        direction = d;
        y = 12;
    }
   
    void print(){
      if(direction == 'r'){
        matrix.drawLine(25, y+4, 25, y+2, matrix.Color333(0, 1, 7));
        matrix.drawLine(26, y+2, 28, y+2, matrix.Color333(2, 0, 7));
        matrix.drawLine(26, y+4, 28, y+5, matrix.Color333(2, 0, 7));
        matrix.drawLine(28, y+2, 28, y  , matrix.Color333(4, 0, 7));
        matrix.drawLine(28, y+4, 28, y+6, matrix.Color333(4, 0, 7));
        matrix.drawLine(28, y+6, 31, y+3, matrix.Color333(6, 1, 7));
        matrix.drawLine(28, y  , 31, y+3, matrix.Color333(6, 1, 7));
        if(moving){
          matrix.drawLine(26, y+3, 30, y+3, matrix.Color333(1, 1, 1));
          matrix.drawLine(29, y+2, 29, y+4, matrix.Color333(1, 1, 1));
        }
      }
      if(direction == 'l'){//y = 25
          matrix.drawLine(0, y+3, 3, y  , matrix.Color333(6, 1, 7));
          matrix.drawLine(0, y+3, 3, y+6, matrix.Color333(6, 1, 7));
          matrix.drawLine(3, y  , 3, y+2, matrix.Color333(4, 0, 7));
          matrix.drawLine(3, y+6, 3, y+4, matrix.Color333(4, 0, 7));
          matrix.drawLine(3, y+4, 6, y+4, matrix.Color333(2, 0, 7));
          matrix.drawLine(3, y+2, 6, y+2, matrix.Color333(2, 0, 7));
          matrix.drawLine(6, y+4, 6, y+2, matrix.Color333(0, 1, 7));
          if(moving){
              matrix.drawLine(1, y+3, 5, y+3, matrix.Color333(1, 1, 1));
              matrix.drawLine(2, y+2, 2, y+4, matrix.Color333(1, 1, 1));
          }
      }
      if(direction == 'u'){//y = 25
          matrix.drawLine(8,  y+3, 11, y, matrix.Color333(6, 1, 7));
          matrix.drawLine(14, y+3, 11, y, matrix.Color333(6, 1, 7));
          matrix.drawLine(8,  y+3, 10, y+3, matrix.Color333(4, 0, 7));
          matrix.drawLine(10, y+3, 10, y+6, matrix.Color333(2, 0, 7));
          matrix.drawLine(12, y+6, 12, y+3, matrix.Color333(2, 0, 7));
          matrix.drawLine(12, y+3, 14, y+3, matrix.Color333(4, 0, 7));
          matrix.drawLine(10, y+6, 12, y+6, matrix.Color333(0, 1, 7));
          if(moving){
              matrix.drawLine(11, y+5, 11, y+1, matrix.Color333(1, 1, 1));
              matrix.drawLine(10, y+2, 12, y+2, matrix.Color333(1, 1, 1));
          }
      }
      if(direction == 'd'){//y = 25
          matrix.drawLine(20, y+6, 23, y+3  , matrix.Color333(6, 1, 7));
          matrix.drawLine(20, y+6, 17, y+3, matrix.Color333(6, 1, 7));
          matrix.drawLine(23, y+3, 21, y+3, matrix.Color333(4, 0, 7));
          matrix.drawLine(17, y+3, 19, y+3, matrix.Color333(4, 0, 7));
          matrix.drawLine(19, y+3, 19, y  , matrix.Color333(2, 0, 7));
          matrix.drawLine(21, y+3, 21, y  , matrix.Color333(2, 0, 7));
          matrix.drawLine(21, y  , 19, y  , matrix.Color333(0, 1, 7));
          if(moving){
              matrix.drawLine(20, y+5, 20, y+1, matrix.Color333(1, 1, 1));
              matrix.drawLine(19, y+4, 21, y+4, matrix.Color333(1, 1, 1));
          }
      }
    }
    void setY(int h){
      y = h;
    }
    void setDir(char d){
      direction = d;
    }
    void setMoving(bool isMoving){
      moving = isMoving;
    }
    int getHeight(){
      return y;
    }
    char getDir(){
      return direction;
    }
};

unsigned char user = 0;
enum GameSM{Init, Start, SongsWait, Songs, Play, HighScore, Quit}game;
void GameSMTick(){
    switch(game){
    case Init:
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        game = Start;
        break;
    case Start:
        //matrix.drawPixel(0, 0, (matrix.Color333(7,0,0)));
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(0, 0);
        matrix.println(">Play");
        matrix.println("<HiS");
        matrix.println("^Quit");
        if((val1 < 20) && (val2 < 20)&&(val3 < 20) && (val4 > 20)){ //select > 1010
            //fillArray();
            game = SongsWait;
        }
        if((val1 > 200) && (val2 > 200)&&(val3 > 200) && (val4 < 20)){ //select < 1001
            game = HighScore;
        }
        if((val1 > 0) && (val1 < 200) && (val2 > 0) && (val2 < 200)&&(val3 > 200) && (val4 < 10)){ //select ^ 1011
            game = Quit;
        }
        break;
    case SongsWait:
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(0, 0);
        matrix.println(">EZ");
        matrix.println("<MED");
        matrix.println("^HARD");
        if(teimer < 5){
            teimer++;
        } else {
          game = Songs;
          teimer = 0;
        }
        break;
    case Songs:
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(0, 0);
        matrix.println(">EZ");
        matrix.println("<MED");
        matrix.println("^HARD");
        if((val1 < 20) && (val2 < 20)&&(val3 < 20) && (val4 > 20)){ //Easy 
            GameArry = noteEZ;
            digitalWrite(39, HIGH);
            game = Play;
        }
        if((val1 > 200) && (val2 > 200)&&(val3 > 200) && (val4 < 20)){ //Medium 
            GameArry = noteMid;
            digitalWrite(40, HIGH);
            game = Play;
        }
        if((val1 > 0) && (val1 < 200) && (val2 > 0) && (val2 < 200)&&(val3 > 200) && (val4 < 10)){ //Hard 
            GameArry = noteHard;
            digitalWrite(41, HIGH);
            game = Play;
        }
        break;
    case Play:
        playGame();
        cycles++;
        if(cycles == GameArry*10){
            if (score > scores[0]){
                scores[1] = scores[0];
                scores[2] = scores[1];
                scores[0] = score;
            } else if ( score > scores[1]){
                scores[2] = scores[1];
                scores[1] = score;
            } else if (score > scores[2]){
                scores[2] = score;
            }
            score = 0;
            game = HighScore;
            cycles = 0;
            //eeprom thingy
        }
        break;
    case HighScore:
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(0, 0);
        matrix.println("Score");
        matrix.println(scores[0]);
        matrix.println(scores[1]);
        matrix.println(scores[2]);
        if((val1 > 0) && (val1 < 200) && (val2 > 0) && (val2 < 200)&&(val3 < 20) && (val4 < 10)){ //b
            game = Start;
        }
        break;
    case Quit: 
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(0, 0);
        matrix.println("Bye!");
        break;
    default:
        break;
    }
}

void playGame(){
    //matrix.fillScreen(matrix.Color333(0, 0, 0));
    int randDir = rand() % 4;
    char tempC = 'r';
    if(randDir == 0){
        tempC = 'r';
    } else if (randDir == 1){
        tempC = 'l';
    } else if (randDir == 2){
        tempC = 'd';
    } else if (randDir == 3){
        tempC = 'u';
    } else {
        tempC = 'r';
    }
    Arrow temp(tempC);
    
    if(tick % 20 == 0){
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        temp.print();
        tick = 0;

    }
    if ((temp.getDir() == 'u') && (val1 > 0) && (val1 < 200) && (val2 > 0) && (val2 < 200)&&(val3 > 200) && (val4 < 10)){
        score = score + 5 - tick%5;
    }
    if ((temp.getDir() == 'd') && (val1 > 0) && (val1 < 200) && (val2 > 0) && (val2 < 200)&&(val3 > 200) && (val4 < 10)){
        score = score + 5 - tick%5;
    }
    if ((temp.getDir() == 'l') && (val1 > 200) && (val2 > 200)&&(val3 > 200) && (val4 < 20)){
        score = score + 5 - tick%5;
    }
    if ((temp.getDir() == 'r') && (val1 < 20) && (val2 < 20)&&(val3 < 20) && (val4 > 20)){
        score = score + 5 - tick%5;
    }
    
    tick++;
}

void setup() {
    srand(0);
    pinMode(39,OUTPUT);
    pinMode(40,OUTPUT);
    pinMode(41,OUTPUT);
    matrix.begin();
    game = Init;
    Serial.begin(9600);
}

void loop(){
    val1 = analogRead(A8);
    val2 = analogRead(A9);
    val3 = analogRead(A10);
    val4 = analogRead(A11); 
    Serial.print(val1);
    Serial.print(',');
    Serial.print(val2);
    Serial.print(',');
    Serial.print(val3);
    Serial.print(',');
    Serial.print(val4);
    Serial.print('\n');

    GameSMTick();
    
    delay(20);
    
}