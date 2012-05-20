-- android glue code

function android_onScreenSizeChanged(width, height)
	print("android_onScreenSizeChanged", width, height)
	love.graphics.setMode(width, height, true, 0)
end
