#include "Probe.h"
#include "includes.h"

#ifdef PORTRAIT_MODE
  #define OFFSET 0
#else
  #define OFFSET 1
#endif

#define GANTRY_UPDATE_DELAY 500  // 1 seconds is 1000


static float currentDia = 0;
static float diaSign = 0;
static uint8_t item_millDia_index = 0;

MENUITEMS probeItems = {
  // title
  LABEL_PROBEMENU, //order: [1st row; 2nd row]
  // icon                          label
  {
    {ICON_ZEROXDIA,                LABEL_ZEROXDIA}, // zero X min dia
    {ICON_ZEROYDIA,                LABEL_ZEROYDIA},        //zero Y min dia
    {ICON_Y_INC,              LABEL_PROBEYPLUS}, // Probe Y + (up)
    {ICON_MILLDIA,                 LABEL_MILLDIA0},     // Select Mill dia
    {ICON_X_DEC,               LABEL_PROBEXMIN}, // Probe X - (left)
    {ICON_X_INC,              LABEL_PROBEXPLUS}, // Probe X + (right)
    {ICON_Y_DEC,               LABEL_PROBEYMIN}, //Probe Y - (down)
    {ICON_BACK,                    LABEL_BACK},       //Back
  }
};


void drawXYZ_four(void)
{
  char tempstr[30];

  GUI_SetColor(infoSettings.status_color);

  sprintf(tempstr, "X:%.2f  ", coordinateGetAxisActual(X_AXIS));
  GUI_DispString(START_X + (OFFSET + 0) * SPACE_X + (OFFSET + 0) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

  sprintf(tempstr, "Y:%.2f  ", coordinateGetAxisActual(Y_AXIS));
  GUI_DispString(START_X + (OFFSET + 1) * SPACE_X + (OFFSET + 1) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

  sprintf(tempstr, "Z:%.2f  ", coordinateGetAxisActual(Z_AXIS));
  GUI_DispString(START_X + (OFFSET + 2) * SPACE_X + (OFFSET + 2) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

  GUI_SetColor(infoSettings.font_color);
}

void updateGantryThree(void)
{
  if (nextScreenUpdate(GANTRY_UPDATE_DELAY))
  {
    coordinateQuery(0);  // query position manually for delay less than 1 second
    drawXYZ_four();
  }
}


void menuProbe(void)
{
  char tempstr[50]; 
  KEY_VALUES key_num = KEY_IDLE;

  currentDia = millDiaSteps[item_millDia_index];
  probeItems.items[KEY_ICON_3] = itemMillDia[item_millDia_index];

  menuDrawPage(&probeItems);
  drawXYZ_four();

  while (MENU_IS(menuProbe))
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0: // zero X min dia
      
        sprintf(tempstr, "G92 X%.3f\n", diaSign*currentDia / 2);        
        storeCmd(tempstr);        
        break; 

      case KEY_ICON_1: //zero Y min dia
        sprintf(tempstr, "G92 Y%.3f\n", diaSign*currentDia / 2);        
        storeCmd(tempstr);   
        break; 
     
      case KEY_ICON_2: diaSign =  -1; storeCmd("G91\n"); storeCmd("G38.2 F300 Y10\n"); storeCmd("G90\n"); break; // Probe Y + (up)
          
      case KEY_ICON_3: // Select Mill dia
          item_millDia_index = (item_millDia_index + 1) % ITEM_ITEM_MILLDIA_NUM;
          probeItems.items[key_num] = itemMillDia[item_millDia_index];
          currentDia = millDiaSteps[item_millDia_index];
          menuDrawItem(&probeItems.items[key_num], key_num);
          break;  
      
      case KEY_ICON_4: diaSign = 1;  storeCmd("G91\n"); storeCmd("G38.2 F300 X-10\n"); storeCmd("G90\n"); break;// Probe X - (left)
      case KEY_ICON_5: diaSign = -1;  storeCmd("G91\n"); storeCmd("G38.2 F300 X10\n"); storeCmd("G90\n"); break;// Probe X + (right)
      case KEY_ICON_6: diaSign = 1;  storeCmd("G91\n"); storeCmd("G38.2 F300 Y-10\n"); storeCmd("G90\n"); break; //Probe Y - (down)
      case KEY_ICON_7: CLOSE_MENU();      break; //Back
      default: break;
    }

    loopProcess();
    updateGantryThree();
  }
}
