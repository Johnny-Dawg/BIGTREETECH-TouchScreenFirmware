#include "Home.h"
#include "includes.h"

#ifdef PORTRAIT_MODE
  #define OFFSET 0
#else
  #define OFFSET 1
#endif

#define GANTRY_UPDATE_DELAY 500  // 1 seconds is 1000

const MENUITEMS homeItems = {
  // title
  LABEL_HOME,
  // icon                          label
  {
    {ICON_HOME_MOVE,               LABEL_HOME_XYZ},
    {ICON_Z_HOME,                  LABEL_Z},
    {ICON_BLTOUCH_DEPLOY,          LABEL_PROBE_Z},
    {ICON_probemenu_XY,            LABEL_PROBEMENU},
    {ICON_ZERO_XY,                 LABEL_ZERO_XY},
    {ICON_ZERO_Z0,                 LABEL_ZERO_Z0},
    {ICON_ZERO_Z0dot5,             LABEL_ZERO_Z0dot5},
    {ICON_BACK,                    LABEL_BACK},
  }
};


void drawXYZ_two(void)
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

void updateGantryTwo(void)
{
  if (nextScreenUpdate(GANTRY_UPDATE_DELAY))
  {
    coordinateQuery(0);  // query position manually for delay less than 1 second
    drawXYZ_two();
  }
}


void menuHome(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&homeItems);
  drawXYZ_two();

  while (MENU_IS(menuHome))
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0: storeCmd("G28\n");   break;
      case KEY_ICON_1: storeCmd("G28 Z\n"); break;
      case KEY_ICON_2: storeCmd("G38.2 Z-10\n"); break;
      case KEY_ICON_3: OPEN_MENU(menuProbe); break;
      

      case KEY_ICON_4: storeCmd("G92 X0 Y0\n"); break;
      case KEY_ICON_5: storeCmd("G92 Z0 \n"); break;
      case KEY_ICON_6: storeCmd("G92 Z0.5 \n"); break;

      case KEY_ICON_7: CLOSE_MENU();      break;
      default: break;
    }

    loopProcess();
    updateGantryTwo();
  }
}
