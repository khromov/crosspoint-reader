#include "BootActivity.h"

#include <Bitmap.h>
#include <GfxRenderer.h>
#include <SDCardManager.h>

#include "fontIds.h"
#include "images/CrossLarge.h"

void BootActivity::onEnter() {
  Activity::onEnter();

  const auto pageWidth = renderer.getScreenWidth();
  const auto pageHeight = renderer.getScreenHeight();

  renderer.clearScreen();

  // Try to load custom logo from SD card
  bool customLogoDrawn = false;
  FsFile file;
  if (SdMan.openFileForRead("BOOT", "/logo.bmp", file)) {
    Bitmap bitmap(file);
    if (bitmap.parseHeaders() == BmpReaderError::Ok) {
      const int logoX = (pageWidth - bitmap.getWidth()) / 2;
      const int logoY = (pageHeight - bitmap.getHeight()) / 2;
      renderer.drawBitmap(bitmap, logoX, logoY, 128, 128);
      customLogoDrawn = true;
    }
    file.close();
  }

  // Fall back to embedded CrossLarge if custom logo not loaded
  if (!customLogoDrawn) {
    renderer.drawImage(CrossLarge, (pageWidth + 128) / 2, (pageHeight - 128) / 2, 128, 128);
  }
  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 + 70, "CrossPoint", true, EpdFontFamily::BOLD);
  renderer.drawCenteredText(SMALL_FONT_ID, pageHeight / 2 + 95, "BOOTING");
  renderer.drawCenteredText(SMALL_FONT_ID, pageHeight - 30, CROSSPOINT_VERSION);
  renderer.displayBuffer();
}
