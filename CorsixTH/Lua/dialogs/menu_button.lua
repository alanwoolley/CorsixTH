local ipairs, math_floor, unpack, select, assert
    = ipairs, math.floor, unpack, select, assert
local TH = require "TH"

--! The ingame menu bar which sits (nominally hidden) at the top of the screen.
class "UIMenuButton" (Window)

local col_bg = {
  red = 154,
  green = 146,
  blue = 198,
}

function UIMenuButton:UIMenuButton(ui)
  self:Window()

  local app = ui.app
  self.ui = ui
  self.world = app.world
  self.on_top = true
  self.resizable = true
  self.draggable = false
  self.esc_closes = false
  self.visible = true
  self.x = 5
  self.y = 5
  self.width = 60
  self.height = 40


  self:addBevelPanel(0, 0, self.width, self.height, col_bg):setLabel("Menu"):makeButton(0, 0, self.width, self.height, nil, self.buttonPressed):setTooltip("Open Menu")

end

function UIMenuButton:buttonPressed()
  print "pressed menu button"
  gamespeedupdated(self.world:getCurrentSpeed())
  showmenu()
  end


