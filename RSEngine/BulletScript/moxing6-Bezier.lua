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
	vetxCount[1] = 200
end

function updateFrame()
	currentFrame = currentFrame + 1
	local tmpt = 1
	local u = 1
	local v = 1
	local gap = 50
		for i=1, 200 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			if (i <= 10) then
				vetx[tmpt] = (400 - currentFrame*2  + 5 * i) 
				u = 0.1
			elseif(i <= 20) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  gap) 
				u = 0.2
			elseif(i <= 30) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 2 * gap) 
				u = 0.3
			elseif (i <= 40) then
				vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 3 * gap) 
				u = 0.4
			elseif(i <= 50) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  4 * gap) 
				u = 0.5
			elseif(i <= 60) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 5 * gap) 
				u = 0.6
			elseif(i <= 70) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 6 * gap) 
				u = 0.7
			elseif (i <= 80) then
				vetx[tmpt] = (400 - currentFrame*2+ 5 * i+ 7 * gap) 
				u = 0.8
			elseif(i <= 90) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  8 * gap) 
				u = 0.9
			elseif(i <= 100) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 9 * gap) 
				u = 1.0
			elseif (i <= 110) then
				vetx[tmpt] = (400 - currentFrame*2  + 5 * i) 
				u = 0.1
			elseif(i <= 120) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  gap) 
				u = 0.2
			elseif(i <= 130) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 2 * gap) 
				u = 0.3
			elseif (i <= 140) then
				vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 3 * gap) 
				u = 0.4
			elseif(i <= 150) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  4 * gap) 
				u = 0.5
			elseif(i <= 160) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 5 * gap) 
				u = 0.6
			elseif(i <= 170) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 6 * gap) 
				u = 0.7
			elseif (i <= 180) then
				vetx[tmpt] = (400 - currentFrame*2+ 5 * i+ 7 * gap) 
				u = 0.8
			elseif(i <= 190) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  8 * gap) 
				u = 0.9
			elseif(i <= 200) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 9 * gap) 
				u = 1.0
            end				
			v = 0.1 * (i % 10)
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 300 -(u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 20) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 30) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 40) then
				vetx[tmpt] = 300 -(u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 50) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 60) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 70) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 80) then
				vetx[tmpt] = 300 -(u * u - 2 * u * v + v * v + v + u) * 500
			elseif(i <= 90) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 100) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif (i <= 110) then
				vetx[tmpt] = -300 + (u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 120) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 130) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 140) then
				vetx[tmpt] = -300 + (u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 150) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 160) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 170) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 180) then
				vetx[tmpt] = -300 + (u * u - 2 * u * v + v * v + v + u) * 500
			elseif(i <= 190) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 200) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			end			
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 20) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 30) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 40) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 50) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 60) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 70) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 80) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 90) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 100) then
				vetx[tmpt] = 300 - 600 * v
			end	
			tmpt = tmpt + 1
		end
end

function updateInterpolation(intp)
	--vetx = {} -- empty array
	local tmpt = 1
	local gap = 50
		for i=1, 200 do 
			--vetx[(i - 1) * 3 + 1] = math.sin(currentFrame - i)
			if (i <= 10) then
				vetx[tmpt] = (400 - currentFrame*2+ 5 * i) - intp
				u = 0.1
			elseif(i <= 20) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  gap)- intp 
				u = 0.2
			elseif(i <= 30) then
			    vetx[tmpt] = (400 - currentFrame*2+ 5 * i + 2 * gap) - intp
				u = 0.3
			elseif (i <= 40) then
				vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 3 * gap) - intp
				u = 0.4
			elseif(i <= 50) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  4 * gap) - intp
				u = 0.5
			elseif(i <= 60) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 5 * gap) - intp
				u = 0.6
			elseif(i <= 70) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 6 * gap) - intp
				u = 0.7
			elseif (i <= 80) then
				vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 7 * gap) - intp
				u = 0.8
			elseif(i <= 90) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  8 * gap) - intp
				u = 0.9
			elseif(i <= 100) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 9 * gap) - intp
				u = 1.0
			elseif (i <= 110) then
				vetx[tmpt] = (400 - currentFrame*2  + 5 * i) 
				u = 0.1
			elseif(i <= 120) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  gap) 
				u = 0.2
			elseif(i <= 130) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 2 * gap) 
				u = 0.3
			elseif (i <= 140) then
				vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 3 * gap) 
				u = 0.4
			elseif(i <= 150) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  4 * gap) 
				u = 0.5
			elseif(i <= 160) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 5 * gap) 
				u = 0.6
			elseif(i <= 170) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 6 * gap) 
				u = 0.7
			elseif (i <= 180) then
				vetx[tmpt] = (400 - currentFrame*2+ 5 * i+ 7 * gap) 
				u = 0.8
			elseif(i <= 190) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+  8 * gap) 
				u = 0.9
			elseif(i <= 200) then
			    vetx[tmpt] = (400 - currentFrame*2 + 5 * i+ 9 * gap) 
				u = 1.0
            end				
			v = 0.1 * (i % 10)
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 300 -(u * u - 2 *u * v + v * v + v + u) * 600
			elseif(i <= 20) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 30) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 40) then
				vetx[tmpt] = 300 -(u * u - 2 *u * v + v * v + v + u) * 600
			elseif(i <= 50) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 60) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 70) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 80) then
				vetx[tmpt] = 300 -(u * u - 2 * u * v + v * v + v + u) * 600
			elseif(i <= 90) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif(i <= 100) then
				vetx[tmpt] = 300 -(u  * u  - 2 * u * v + v * v + v + u ) * 600
			elseif (i <= 110) then
				vetx[tmpt] = -300 + (u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 120) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 130) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 140) then
				vetx[tmpt] = -300 + (u * u - 2 *u * v + v * v + v + u) * 500
			elseif(i <= 150) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 160) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 170) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 180) then
				vetx[tmpt] = -300 + (u * u - 2 * u * v + v * v + v + u) * 500
			elseif(i <= 190) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			elseif(i <= 200) then
				vetx[tmpt] = -300 + (u  * u  - 2 * u * v + v * v + v + u ) * 500
			end			
			tmpt = tmpt + 1
			if (i <= 10) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 20) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 30) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 40) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 50) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 60) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 70) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 80) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 90) then
				vetx[tmpt] = 300 - 600 * v
			elseif(i <= 100) then
				vetx[tmpt] = 300 - 600 * v
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
