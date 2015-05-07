print(math.sin( math.rad(30)) );

local atan_rad_table = {};
local pi = math.pi;

--[[
for i=0,20 do
	print('~',i,180*1/(2^i)/pi,math.atan( 1/2^i),math.cos(math.atan( 1/2^i)) )
	atan_rad_table[i] = math.atan( 1/2^i);
end
--]]

atan_rad_table = {
	[0] = 0.78539816339745,
	[1] = 0.46364760900081,
	[2] = 0.24497866312686,
	[3] = 0.12435499454676,
	[4] = 0.062418809995957,
	[5] = 0.031239833430268,
	[6] = 0.015623728620477,
    [7] = 0.0078123410601011,
    [8] = 0.003906230131967,
    [9] = 0.0019531225164788,
    [10] = 0.00097656218955932,
	[11] = 0.0004882812111949,
	[12] = 0.00024414062014936,
	[13] = 0.00012207031189367,
	[14] = 6.1035156174209e-005,
	[15] = 3.0517578115526e-005,
	[16] = 1.5258789061316e-005,
	[17] = 7.629394531102e-006,
	[18] = 3.8146972656065e-006,
	[19] = 1.9073486328102e-006,
	[20] = 9.5367431640596e-007,
}

local accu_scale_table = {};
accu_scale_table = {
	[0] = 0.70710678118655,
    [1] = 0.63245553203368,
    [2] = 0.6135719910779,
    [3] = 0.60883391251775,
    [4] = 0.60764825625617,
    [5] = 0.6073517701413,
    [6] = 0.60727764409353,
    [7] = 0.60725911229889,
    [8] = 0.60725447933256,
    [9] = 0.60725332108988,
    [10] = 0.60725303152913,
    [11] = 0.60725295913894,
    [12] = 0.6072529410414,
    [13] = 0.60725293651701,
    [14] = 0.60725293538591,
    [15] = 0.60725293510314,
    [16] = 0.60725293503245,
    [17] = 0.60725293501477,
    [18] = 0.60725293501035,
    [19] = 0.60725293500925,
    [20] = 0.60725293500897,
}

for i=0,20 do
	local acc = 1;
	for ii=0,i do
		acc = acc * 1/math.sqrt(1+(1/2^ii)^2);
	end
	--accu_scale_table[i] = acc;
	--print(i,'acc:',acc);
	print('cos',1/math.sqrt(1+(1/2^i)^2) .. ',');
end

--[[
x1 = cos(��)* x0		-	sin(��)*y0
y1 = sin(��)* x0		+	cos(��)*y0

x1 = cos(��)*(	 x0				-		y0*tan(��)	)
y1 = cos(��)*(	 x0*tan(��)		+		y0			)


Xn+1 = COS(��n-1 + ��n)*Xn-1 - SIN(��n-1 + ��n)*Yn-1
Yn+1 = SIN(��n-1 + ��n)*Xn-1 + COS(��n-1 + ��n)*Yn-1

]]--
function cordic(rad)
	print('����:',rad)
	print('������tan:',math.tan(rad),'cos',math.cos(rad),'sin',math.sin(rad));
	local rad_index = 0;
	local z = atan_rad_table[0];--��ʼʱΪ��һ������math.atan( 1/2^0)������0.78539816339745

	local curr_x = 1
	local curr_y = 0
	local d = 1;
	local iter = 0;
	while iter < 20 do
		if z > rad then
			d = -1;
		elseif z > rad then
			d = 1;
		end
		--print('d',d,'Ŀ�ĽǶ�:',rad,'��ǰ�Ƕ�:',z,'���ϽǶ�:',d*atan_rad_table[iter]);
		local x = curr_x 				- 	d*curr_y * (1/(2^iter));
		local y = d*curr_x*(1/(2^iter)) +	curr_y;
		z = z + d*atan_rad_table[iter];
		--print('--------------------');
		--print('x',x,'y',y)
		--print('rad',rad)
		--print('iter',iter)
		--print('z',z);
		curr_x = x;
		curr_y = y;
		iter = iter + 1;
	end

	print('���ս������','curr_x:',curr_x,'curr_y:',curr_y,'z:',z,'����rad��',rad);
	print('����ֵaccu_scale::',accu_scale_table[iter]);
	print('������x��y',curr_x*accu_scale_table[iter],curr_y * accu_scale_table[iter]);
	--print('cos',curr_x,accu_scale_table[iter],curr_x * accu_scale_table[iter]);
end

local pi = math.pi;

cordic( 30 / 180 * pi);





