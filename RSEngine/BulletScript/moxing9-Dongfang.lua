-- procedure
-- rsengine calls updateFrame() to move the vertex system to a new frame
-- rsengine calls updateInterpoloation() to work out the current position of vertexes with interpolation
-- rsengine get strip count and vetxCount to featch vertexes in vetx 

-- the start coordination is 0, and bullets go to -x direction


-- the template implements a simple bullet storm
-- x = sin(t) * 100
-- y = cos(t) * 100
-- z = t
-- stripCount == 1
-- vetxCount[1] = 50

function initialize()
	vetx = {} -- empty array
	stripCount = 1 -- modify here
	vetxCount = {} -- empty array to store vertex count for each strip
	currentFrame = 0 -- current frame
	maxFrame = 1200 -- max frame count, if current frame is bigger than maxFrame, then the bulletstorm will be removed from system
	lineCount = 0 -- line count
	line = {} -- stotre line index in vetx
	lineUpdated = 0 -- 0 for not updated since last call 1 for updated since last call
	stripCount = 1
	vetxCount[1] = 400
end

function updateFrame()
	currentFrame = currentFrame + 1/ 2
	local tmpt = 1
	local gap = 90
	for i=1, 400 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			if (i <= 40) then
				vetx[tmpt] = 400 - currentFrame 
			elseif(i <= 80) then
				vetx[tmpt] = 400 - currentFrame  + gap * 2 
			elseif(i <= 120) then
				vetx[tmpt] = 400 - currentFrame  + gap 
			elseif(i <= 160) then
				vetx[tmpt] = 400 - currentFrame  + gap * 3
			elseif(i <= 200) then
				vetx[tmpt] = 400 - currentFrame  + gap * 2
			elseif(i <= 240) then
				vetx[tmpt] = 400 - currentFrame  + gap * 4 
			elseif(i <= 280) then
				vetx[tmpt] = 400 - currentFrame  + gap * 3
			elseif(i <= 320) then
				vetx[tmpt] = 400 - currentFrame  + gap * 5 
			elseif(i <= 360) then
				vetx[tmpt] = 400 - currentFrame  + gap * 4 
			elseif(i <= 400) then
				vetx[tmpt] = 400 - currentFrame  + gap * 5 
			end			
			
			tmpt = tmpt + 1
			if (i <= 20) then
				vetx[tmpt] = -300 + 14 * i
			elseif (i <= 40) then
				vetx[tmpt] = 20 + 14 * (i - 20)
			elseif(i <= 80) then
				vetx[tmpt] = 20
			elseif (i <= 110) then
				vetx[tmpt] = -300 + 14 * (i - 80)
			elseif (i <= 120) then
				vetx[tmpt] = 160 + 14 * (i - 110)
			elseif (i <= 160) then
				vetx[tmpt] = 20
			elseif(i <= 180) then
				vetx[tmpt] = -300 + 14 * (i - 160)
			elseif(i <= 200) then
				vetx[tmpt] = 20 + 14 * (i - 180)
			elseif(i <= 240) then
				vetx[tmpt] = -20
			elseif(i <= 250) then
				vetx[tmpt] = -300 + 14 * (i - 240)
			elseif(i <= 280) then
				vetx[tmpt] = -120 + 14 * (i - 250)
			elseif(i <= 320) then
				vetx[tmpt] = -20
			elseif(i <= 340) then
				vetx[tmpt] = -300 + 14 * (i - 320)
			elseif(i <= 360) then
				vetx[tmpt] = 20 + 14 * (i - 340)
			elseif(i <= 400) then
				vetx[tmpt] = -300 + 14 * (i - 360)	
			end				
			tmpt = tmpt + 1
			if (i <= 40) then
				vetx[tmpt] = 0 
			elseif (i <= 60) then
				vetx[tmpt] = -300 + 14 * (i - 40)
			elseif (i <= 80) then
				vetx[tmpt] = 40 + 14 * (i - 60)
			elseif(i <= 120) then
				vetx[tmpt] = 0
			elseif (i <= 150) then
				vetx[tmpt] = -300 + 14 * (i - 120)
			elseif (i <= 160) then
				vetx[tmpt] = 180 + 14 * (i - 150)				
			elseif(i <= 200) then
				vetx[tmpt] = 0
			elseif(i <= 220) then
				vetx[tmpt] = -300 + 14 * (i - 200)
			elseif(i <= 240) then
				vetx[tmpt] = 40 + 14 * (i - 220)
			elseif(i <= 280) then
				vetx[tmpt] = 0
			elseif(i <= 290) then
				vetx[tmpt] = -300 + 14 * (i - 280)
			elseif(i <= 320) then
				vetx[tmpt] = -100 + 14 * (i - 290)
			elseif(i <= 360) then
				vetx[tmpt] = 0
			elseif(i <= 400) then
				vetx[tmpt] = 0
			end	
			tmpt = tmpt + 1
		end
end

function updateInterpolation(intp)
	--vetx = {} -- empty array
	currentFrame = currentFrame + 1/ 2
	local tmpt = 1
	local gap = 90
		for i=1, 400 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			if (i <= 40) then
				vetx[tmpt] = 400 - currentFrame  - intp
			elseif(i <= 80) then
				vetx[tmpt] = 400 - currentFrame  + gap * 2 - intp
			elseif(i <= 120) then
				vetx[tmpt] = 400 - currentFrame  + gap - intp
			elseif(i <= 160) then
				vetx[tmpt] = 400 - currentFrame  + gap * 3 - intp
			elseif(i <= 200) then
				vetx[tmpt] = 400 - currentFrame  + gap * 2 -intp
			elseif(i <= 240) then
				vetx[tmpt] = 400 - currentFrame  + gap * 4 -intp
			elseif(i <= 280) then
				vetx[tmpt] = 400 - currentFrame  + gap * 3 -intp
			elseif(i <= 320) then
				vetx[tmpt] = 400 - currentFrame  + gap * 5 -intp
			elseif(i <= 360) then
				vetx[tmpt] = 400 - currentFrame  + gap * 4 -intp
			elseif(i <= 400) then
				vetx[tmpt] = 400 - currentFrame  + gap * 5 -intp
			end				
			tmpt = tmpt + 1
			if (i <= 20) then
				vetx[tmpt] = -300 + 14 * i
			elseif (i <= 40) then
				vetx[tmpt] = 20 + 14 * (i - 20)
			elseif(i <= 80) then
				vetx[tmpt] = 20
			elseif (i <= 110) then
				vetx[tmpt] = -300 + 14 * (i - 80)
			elseif (i <= 120) then
				vetx[tmpt] = 160 + 14 * (i - 110)
			elseif (i <= 160) then
				vetx[tmpt] = 20
			elseif(i <= 180) then
				vetx[tmpt] = -300 + 14 * (i - 160)
			elseif(i <= 200) then
				vetx[tmpt] = 20 + 14 * (i - 180)
			elseif(i <= 240) then
				vetx[tmpt] = -20
			elseif(i <= 250) then
				vetx[tmpt] = -300 + 14 * (i - 240)
			elseif(i <= 280) then
				vetx[tmpt] = -120 + 14 * (i - 250)
			elseif(i <= 320) then
				vetx[tmpt] = -20
			elseif(i <= 340) then
				vetx[tmpt] = -300 + 14 * (i - 320)
			elseif(i <= 360) then
				vetx[tmpt] = 20 + 14 * (i - 340)
			elseif(i <= 400) then
				vetx[tmpt] = -300 + 14 * (i - 360)
			end				
			tmpt = tmpt + 1
			if (i <= 40) then
				vetx[tmpt] = 0 
			elseif (i <= 60) then
				vetx[tmpt] = -300 + 14 * (i - 40)
			elseif (i <= 80) then
				vetx[tmpt] = 40 + 14 * (i - 60)
			elseif(i <= 120) then
				vetx[tmpt] = 0
			elseif (i <= 150) then
				vetx[tmpt] = -300 + 14 * (i - 120)
			elseif (i <= 160) then
				vetx[tmpt] = 180 + 14 * (i - 150)				
			elseif(i <= 200) then
				vetx[tmpt] = 0
			elseif(i <= 220) then
				vetx[tmpt] = -300 + 14 * (i - 200)
			elseif(i <= 240) then
				vetx[tmpt] = 40 + 14 * (i - 220)
			elseif(i <= 280) then
				vetx[tmpt] = 0
			elseif(i <= 290) then
				vetx[tmpt] = -300 + 14 * (i - 280)
			elseif(i <= 320) then
				vetx[tmpt] = -100 + 14 * (i - 290)
			elseif(i <= 360) then
				vetx[tmpt] = 0
			elseif(i <= 400) then
				vetx[tmpt] = 0
			end	
			tmpt = tmpt + 1
		end
end

function isDone()
	if (currentFrame > maxFrame) then
		return 1
	end;
	return 0
end
