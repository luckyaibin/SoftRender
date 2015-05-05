#ifndef __TOOL_H__
#define __TOOL_H__
//�Ѹ�����ת���ɶ���������ͨ��*(1<<fracbits)�ķ�ʽ��ֻͨ����λ������fracbits�Ƕ�����ƫ��
int float_to_fixpoint(float fval,int fracbits)
{
	int ival = *(int *)&fval;
	// ��ȡβ��,ע��ʵ�ʵ�β��ǰ�滹��һ����ʡ�Ե���1
	int mantissa = (ival & 0x007fffff) | 0x800000;
	// ��ȡָ��,��23�ֽ�
	int exponent =  (ival >> 23) & 0xff;
	exponent = exponent - 127;//�õ�������ָ��ֵ
	if ( (fracbits+exponent) < 23)
		mantissa = mantissa>>( 23 - (fracbits+exponent));
	else
		mantissa = mantissa<<(  (fracbits+exponent) - 23);
	// ���С��0���򽫽��ȡ��
	if ((*(int *)&fval) & 0x80000000)
		mantissa = -mantissa;
	//int vvv = fval * (1<<fracbits);
	return mantissa;
}
//�Ѷ�����ת���ɸ�����
float fixpoint_to_float(int i,int fractbits)
{
	float f;
	int *p = (int*)&f;
	*p=0;
	int is_negative=0;
	if (i<0)
	{
		is_negative = 1;
		i=-i;
	}
	//���λbit����λ��
	int highest_bit_pos = 31;
	while (!((1<<highest_bit_pos) & i))
		highest_bit_pos--;
	int exponent = highest_bit_pos-fractbits;
	exponent = exponent + 127;
	//ȥ�����λ��1
	int mantissa = (i & (  ~(1<<highest_bit_pos) ));
	//�ٰ����λ�ұ��Ǹ�bit��λ����23��bit��λ��,����
	if (highest_bit_pos-1<22)
		mantissa = mantissa << (22 - (highest_bit_pos - 1));
	else
		mantissa = mantissa >> ( (highest_bit_pos-1) - 22 );
	*p = exponent;
	*p = (*p << 23);
	*p = (*p)|mantissa;
	if (is_negative)
		f = -f;
	return f;
}
#endif