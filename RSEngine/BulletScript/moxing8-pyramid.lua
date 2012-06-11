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
	vetxCount[1] = 320
end

function updateFrame()
	currentFrame = currentFrame + 1 / 2
	local tmpt = 1
	local xgap = 200
		for i=1, 320 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			--竖边
			if (i <= 10) then
				vetx[tmpt] = 400 - currentFrame +  30 * i
			elseif(i <= 20) then
			    vetx[tmpt] = 400 - currentFrame + xgap +  10 * (i - 10)
			elseif(i <= 30) then
			    vetx[tmpt] = 400 - currentFrame + 2 * xgap - 10 * (i - 20)
			elseif(i <= 40) then
			    vetx[tmpt] = 400 - currentFrame + 3 * xgap - 30 * (i - 30)
			--下中间横边	
			elseif(i <= 50) then
				vetx[tmpt] = 400 - currentFrame + 15 * (i - 40) + 200
			elseif(i <= 60) then
			    vetx[tmpt] = 400 - currentFrame + 15 * (i - 50) + xgap + 60
			--上中间横边
			elseif(i <= 70) then
				vetx[tmpt] = 400 - currentFrame + 30 * (i - 60) + 50
			elseif(i <= 80) then
			    vetx[tmpt] = 400 - currentFrame + 30 * (i - 70) + xgap + 30
			--上下横边
			elseif (i <= 100) then
				vetx[tmpt] = 400 - currentFrame + 20 * (i - 80)
			elseif(i <= 120) then
			    vetx[tmpt] = 400 - currentFrame + 20 * (i - 100) + xgap
			elseif(i <= 140) then
			    vetx[tmpt] = 400 - currentFrame  + 300
			elseif(i <= 160) then
			    vetx[tmpt] = 400 - currentFrame  + 300
			--左右横边
			elseif (i <= 180) then
				vetx[tmpt] = 400 - currentFrame + 10 * (i - 160)
			elseif(i <= 200) then
			    vetx[tmpt] = 400 - currentFrame + 10 * (i - 180) + 2 * xgap
			elseif(i <= 220) then
			    vetx[tmpt] = 400 - currentFrame  + 300
			elseif(i <= 240) then
			    vetx[tmpt] = 400 - currentFrame  + 300
			--下中间左右横边	
			elseif (i <= 250) then
				vetx[tmpt] = 400 - currentFrame + 5 * (i - 240) + 200
			elseif(i <= 260) then
			    vetx[tmpt] = 400 - currentFrame + 5 * (i - 250) +  350
			--上中间左右横边	
			elseif (i <= 270) then
				vetx[tmpt] = 400 - currentFrame + 20 * (i - 260) + 50
			elseif(i <= 280) then
			    vetx[tmpt] = 400 - currentFrame + 20 * (i - 270) + 350 
            end						
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 220 - i * 47
			elseif(i <= 20) then
				vetx[tmpt] = 280 - (i - 10 ) * 53
			elseif(i <= 30) then
				vetx[tmpt] = 220 - (i - 20) * 47
			elseif(i <= 40) then
				vetx[tmpt] = 280 - (i - 30) * 53
			elseif(i <= 50) then
				vetx[tmpt] = -130 
			elseif(i <= 60) then
				vetx[tmpt] = -70
			elseif(i <= 70) then
				vetx[tmpt] = 70 
			elseif(i <= 80) then
				vetx[tmpt] = 130	
			elseif(i <= 100) then
				vetx[tmpt] = 220 
			elseif(i <= 120) then
				vetx[tmpt] = 280 
			elseif(i <= 140) then
				vetx[tmpt] = -280 + currentFrame - 70 * (i - 140)
			elseif(i <= 160) then
				vetx[tmpt] = -280 + currentFrame - 70 * (i - 140)
			elseif(i <= 180) then
				vetx[tmpt] = 220 + (i - 160) * 3
			elseif(i <= 200) then
				vetx[tmpt] = 220 + (i - 180) * 3 
			elseif(i <= 220) then
				vetx[tmpt] = -250 - (i - 200) * 3
			elseif(i <= 240) then
				vetx[tmpt] = -250 - (i - 220) * 3
			elseif(i <= 250) then
				vetx[tmpt] = -130 + (i - 240) * 5
			elseif(i <= 260) then
				vetx[tmpt] = -130 + (i - 250) * 5
			elseif(i <= 270) then
				vetx[tmpt] = 70 + (i - 260) * 6
			elseif(i <= 280) then
				vetx[tmpt] = 70 + (i - 270) * 6
			end			
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 280 - 28 * i
			elseif(i <= 20) then
				vetx[tmpt] = -280 + 28 * ( i - 10)
			elseif(i <= 30) then
				vetx[tmpt] = 280 - 28 * (i - 20)
			elseif(i <= 40) then
				vetx[tmpt] = -280 + 28 * (i - 30)
			elseif(i <= 50) then
				vetx[tmpt] = 90 
			elseif(i <= 60) then
				vetx[tmpt] = -90
			elseif(i <= 70) then
				vetx[tmpt] = 180 
			elseif(i <= 80) then
				vetx[tmpt] = -180
			elseif(i <= 100) then
				vetx[tmpt] = 280 
			elseif(i <= 120) then
				vetx[tmpt] = -280 
			elseif(i <= 140) then
				vetx[tmpt] = 0
			elseif(i <= 160) then
				vetx[tmpt] = 0
			elseif(i <= 180) then
				vetx[tmpt] = 280 - 28 * (i - 160) 
			elseif(i <= 200) then
				vetx[tmpt] = 280 - 28 * (i - 180) 
			elseif(i <= 220) then
				vetx[tmpt] = 0
			elseif(i <= 240) then
				vetx[tmpt] = 0
			elseif(i <= 250) then
				vetx[tmpt] = 90 - 18 * (i - 240) 
			elseif(i <= 260) then
				vetx[tmpt] = 90 - 18 * (i - 250)
			elseif(i <= 270) then
				vetx[tmpt] = 180 - 36 * (i - 260) 
			elseif(i <= 280) then
				vetx[tmpt] = 180 - 36 * (i - 270)
			end	
			tmpt = tmpt + 1
		end
end

function updateInterpolation(intp)
	--vetx = {} -- empty array
	currentFrame = currentFrame + 1 / 2
	local tmpt = 1
	local xgap = 200
		for i=1, 320 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			if (i <= 10) then
				vetx[tmpt] = 400 - currentFrame +  30 * i- intp
			elseif(i <= 20) then
			    vetx[tmpt] = 400 - currentFrame + xgap +  10 * (i - 10)- intp
			elseif(i <= 30) then
			    vetx[tmpt] = 400 - currentFrame + 2 * xgap - 10 * (i - 20)- intp
			elseif(i <= 40) then
			    vetx[tmpt] = 400 - currentFrame + 3 * xgap - 30 * (i - 30)- intp
			elseif(i <= 50) then
				vetx[tmpt] = 400 - currentFrame + 15 * (i - 40) - intp + 200
			elseif(i <= 60) then
			    vetx[tmpt] = 400 - currentFrame + 15 * (i - 50) + xgap - intp + 60
			elseif(i <= 70) then
				vetx[tmpt] = 400 - currentFrame + 30 * (i - 60) - intp + 50
			elseif(i <= 80) then
			    vetx[tmpt] = 400 - currentFrame + 30 * (i - 70) + xgap - intp + 30
			elseif (i <= 100) then
				vetx[tmpt] = 400 - currentFrame + 20 * (i - 80) - intp
			elseif(i <= 120) then
			    vetx[tmpt] = 400 - currentFrame + 20 * (i - 100) + xgap - intp
			elseif(i <= 140) then
			    vetx[tmpt] = 400 - currentFrame + 300 - intp
			elseif(i <= 160) then
			    vetx[tmpt] = 400 - currentFrame + 300 - intp
			elseif (i <= 180) then
				vetx[tmpt] = 400 - currentFrame + 10 * (i - 160) - intp
			elseif(i <= 200) then
			    vetx[tmpt] = 400 - currentFrame + 10 * (i - 180) + 2 * xgap - intp
			elseif(i <= 220) then
			    vetx[tmpt] = 400 - currentFrame  + 300 - intp
			elseif(i <= 240) then
			    vetx[tmpt] = 400 - currentFrame  + 300 - intp
			elseif (i <= 250) then
				vetx[tmpt] = 400 - currentFrame + 5 * (i - 240) + 200 - intp
			elseif(i <= 260) then
			    vetx[tmpt] = 400 - currentFrame + 5 * (i - 250) +  350 - intp
			elseif (i <= 270) then
				vetx[tmpt] = 400 - currentFrame + 20 * (i - 260) - intp + 50
			elseif(i <= 280) then
			    vetx[tmpt] = 400 - currentFrame + 20 * (i - 270) + 350 - intp
            end				
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 220 - i * 47
			elseif(i <= 20) then
				vetx[tmpt] = 280 - (i - 10 ) * 53
			elseif(i <= 30) then
				vetx[tmpt] = 220 - (i - 20) * 47
			elseif(i <= 40) then
				vetx[tmpt] = 280 - (i - 30) * 53
			elseif(i <= 50) then
				vetx[tmpt] = -130 
			elseif(i <= 60) then
				vetx[tmpt] = -70
			elseif(i <= 70) then
				vetx[tmpt] = 70 
			elseif(i <= 80) then
				vetx[tmpt] = 130
			elseif(i <= 100) then
				vetx[tmpt] = 220 
			elseif(i <= 120) then
				vetx[tmpt] = 280 
			elseif(i <= 140) then
				vetx[tmpt] = -280 + currentFrame - 70 * (i - 140)
			elseif(i <= 160) then
				vetx[tmpt] = -280 + currentFrame - 70 * (i - 140)
			elseif(i <= 180) then
				vetx[tmpt] = 220 + (i - 160) * 3
			elseif(i <= 200) then
				vetx[tmpt] = 220 + (i - 180) * 3 
			elseif(i <= 220) then
				vetx[tmpt] = -250 - (i - 200) * 3
			elseif(i <= 240) then
				vetx[tmpt] = -250 - (i - 220) * 3
			elseif(i <= 250) then
				vetx[tmpt] = -130 + (i - 240) * 5
			elseif(i <= 260) then
				vetx[tmpt] = -130 + (i - 250) * 5
			elseif(i <= 270) then
				vetx[tmpt] = 70 + (i - 260) * 6
			elseif(i <= 280) then
				vetx[tmpt] = 70 + (i - 270) * 6
			end			
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 280 - 28 * i
			elseif(i <= 20) then
				vetx[tmpt] = -280 + 28 * ( i - 10)
			elseif(i <= 30) then
				vetx[tmpt] = 280 - 28 * (i - 20)
			elseif(i <= 40) then
				vetx[tmpt] = -280 + 28 * (i - 30)
			elseif(i <= 50) then
				vetx[tmpt] = 90 
			elseif(i <= 60) then
				vetx[tmpt] = -90
			elseif(i <= 70) then
				vetx[tmpt] = 180 
			elseif(i <= 80) then
				vetx[tmpt] = -180
			elseif(i <= 100) then
				vetx[tmpt] = 280 
			elseif(i <= 120) then
				vetx[tmpt] = -280 
			elseif(i <= 140) then
				vetx[tmpt] = 0
			elseif(i <= 160) then
				vetx[tmpt] = 0
			elseif(i <= 180) then
				vetx[tmpt] = 280 - 28 * (i - 160) 
			elseif(i <= 200) then
				vetx[tmpt] = 280 - 28 * (i - 180) 
			elseif(i <= 220) then
				vetx[tmpt] = 0
			elseif(i <= 240) then
				vetx[tmpt] = 0
			elseif(i <= 250) then
				vetx[tmpt] = 90 - 18 * (i - 240) 
			elseif(i <= 260) then
				vetx[tmpt] = 90 - 18 * (i - 250)
			elseif(i <= 270) then
				vetx[tmpt] = 180 - 36 * (i - 260) 
			elseif(i <= 280) then
				vetx[tmpt] = 180 - 36 * (i - 270)
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
