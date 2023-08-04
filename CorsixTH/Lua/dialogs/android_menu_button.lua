class "UIAndroidMenuButton"(Window)

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
    self.menu_open = false

    self.android_menu = UIAndroidMenu(self.ui)

    self.panel_sprites = self.app.gfx:loadSpriteTable("Bitmap", "android", true, self.app.gfx:loadPalette("Bitmap", "android.pal"))

    self:addPanel(12, 65, 0, self.width, self.height):setLabel("Menu"):makeButton(0, 0, self.width, self.height, 13, self.buttonPressed):setTooltip("Open Menu")

end

function UIAndroidMenuButton:buttonPressed()
    if (not self.menu_open) then
        print("Opening menu")
        self:addWindow(self.android_menu)
        self.menu_open = true
    else
        print("Closing menu")
        self:removeWindow(self.android_menu)
        self.menu_open = false
    end
end

function UIAndroidMenuButton:onChangeResolution(width, height)
    self.x = self.ui.app.config.width - 100
end