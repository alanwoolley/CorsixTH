class "UIAndroidMenu" (UIResizable)

--@type UIAndroidMenu
local UIAndroidMenu = _G["UIAndroidMenu"]

local TH = require("TH")

local col_bg = {
  red = 154,
  green = 146,
  blue = 198,
}

local game_speeds = { "Slowest", "Slower", "Normal", "Max speed", "And then some more" }
local speed_sprite_index_offset = 6

function UIAndroidMenu:UIAndroidMenu(ui)
    self:UIResizable(ui, 81, 378, col_bg)

    self.app = ui.app

    self.resizable = true
    self.draggable = false
    self.esc_closes = false

    self:setDefaultPosition(0, 50)

    self.speed = 1
    self.paused = self.app.world:isPaused()

    local current_speed = self.app.world:getCurrentSpeed()

    for i=1, #game_speeds do
      if game_speeds[i] == current_speed then
        self.speed = i
      end
    end

    self.panel_sprites = self.app.gfx:loadSpriteTable("Bitmap", "android", true, self.app.gfx:loadPalette("Bitmap", "android.pal"))
    self.pause_button = self:addPanel( 3,   0, 0):makeToggleButton(0, 0, 81, 54, 4, self.onPausedPressed, nil, nil)
    self.speed_button = self:addPanel(speed_sprite_index_offset + self.speed,   0, 54):makeButton(0, 0, 81, 54, speed_sprite_index_offset + self.speed, self.onSpeedClicked, nil, nil)
    self.save_button = self:addPanel( 5,   0, 108):makeButton(0, 0, 81, 54, 6, self.onSavePressed, nil, nil)
    self.load_button = self:addPanel( 1,   0, 162):makeButton(0, 0, 81, 54, 2, self.onLoadPressed, nil, nil)
    self.settings = self:addPanel( 14,   0, 216):makeButton(0, 0, 81, 54, 15, self.onSettingsPressed, nil, nil)
    self.restart = self:addPanel( 18,   0, 270):makeButton(0, 0, 81, 54, 19, self.onRestartPressed, nil, nil)
    self.exit = self:addPanel( 16,   0, 324):makeButton(0, 0, 81, 54, 17, self.onExitPressed, nil, nil)

end

function UIAndroidMenu:onSpeedClicked(window)
  local new_speed = self.speed + 1
  if (new_speed > #game_speeds) then
    new_speed = 1
  end

  self.speed = new_speed
  self.speed_button.sprite_index_active = speed_sprite_index_offset + new_speed
  self.speed_button.panel_for_sprite.sprite_index = speed_sprite_index_offset + new_speed
  self.app.world:setSpeed(game_speeds[new_speed])
  self.paused = false
  self.pause_button:setToggleState(false)
end

function UIAndroidMenu:onPausedPressed(paused)
  self.app.world:pauseOrUnpause()
  self.paused = self.app.world:isPaused()
end

function UIAndroidMenu:onExitPressed()
  self.app.ui:quit()
end

function UIAndroidMenu:onRestartPressed()
  self.app:restart()
end

function UIAndroidMenu:onSettingsPressed()
    TH.android.showSettings()
end

function UIAndroidMenu:onLoadPressed()
    TH.android.showLoad()
end

function UIAndroidMenu:onSavePressed()
    TH.android.showSave()
end