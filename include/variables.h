#define LED 4

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200

#define AD_DIR 21
#define AD_STEP 19
#define DEC_DIR 14
#define DEC_STEP 16

#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000

#define MS1 10
#define MS2 11
#define MS3 12


int microsteps = 1;
int rpm = 100;

//const char *ssid = "Bbox-5EEDBB66";
//const char *password = "MuShu2020";

double actual_da = 0;
double goto_da = 0;
double actual_dec = 0;
double goto_dec = 0;
double actual_ha = 0;
double target_ha = 0;

double longitude = 2.35;
double latitude = 48.89;
double altitude = 31;

double stepCoef = 10;
bool move_da = false;
bool move_dec = false;

#define RPM 120
#define MICROSTEPS 16
#define RAPPORT_POULIE 216

bool wifiConnected = false;
bool filesystem = false;