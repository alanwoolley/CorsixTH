class "UIAndroidMenuButton" (Window)

local col_bg = {
  red = 154,
  green = 146,
  blue = 198,
}

function UIAndroidMenuButton:UIAndroidMenuButton(ui)
  self:Window()

  local app = ui.app
  self.app = app
  self.ui = ui
  self.world = app.world
  self.on_top = true
  self.resizable = true
  self.draggable = false
  self.esc_closes = false
  self.visible = true
  self.x = app.config.width - 100
  self.y = 10
  self.width = 27
  self.height = 28

  self.android_menu = UIAndroidMenu(self.ui)
  self.android_menu.visible = false
  self:addWindow(self.android_menu)

  self.panel_sprites = self.app.gfx:loadSpriteTable("Bitmap", "android", true, self.app.gfx:loadPalette("Bitmap", "android.pal"))

  self:addPanel(12, 65, 0, self.width, self.height):setLabel("Menu"):makeButton(0, 0, self.width, self.height, 13, self.buttonPressed):setTooltip("Open Menu")

end

function UIAndroidMenuButton:buttonPressed()
  print "pressed menu button"
  self.android_menu.visible = not self.android_menu.visible
end

function UIAndroidMenuButton:onChangeResolution(width, height)
    self.x = self.ui.app.config.width - 100
end