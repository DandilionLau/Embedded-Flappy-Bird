/* Դ�ļ� touch.c */

/*
    ����:���败������У׼����V1.0��
    ����:�����仪

    ��ֲ��ʱ��ֻҪ�� Touch_Structure �����������ֵ �Ϳ�����

    Get_XChannel        �õ�Xͨ�������ݵķ���

    Get_YChannel        �õ�Yͨ�����ݵķ���

    Delay_ms            ���뼶��ʱ����

    Get_StatusFlag      ��ⴥ���������µķ��� 0 û�а���  1 �ѽᰴ����

    Clear_PendingBit    ������������±�־�ķ���

    DispH               ��Һ��������ʾʮ�ֺŵķ���

    ��ֲ֮ǰҪ��ʵ���⼸���򵥵ĺ���/����
*/

#include "touch.h"

#define N   10
/* ������һ�δ����� �ɼ������� */
static uint16_t TP_Coordinate[2][10];

/* ��Touch_Structure ��������ֵ */
static Tuch_TypeDef   Touch_Structure;


#define R   50
/* �����������˲� */

int ifTouch()
{
	return Touch_Structure.Get_StatusFlag();
}
uint16_t TP_Data_Processing(uint16_t *pData,uint8_t Length)
{
    uint8_t i,j,Cnt=0,Max=0;

    uint16_t Number,cc;

    for(i=0;i<Length;i++)
    {
        for(j=0;j<Length;j++)
        {
            if(i==j)
                continue;

            if(pData[i]>pData[j])
                cc=pData[i]-pData[j];
            else
                cc=pData[j]-pData[i];

            if(cc<R)
            {
                Cnt++;
            }
        }

        if(Cnt>Max)
        {
            Max=Cnt;
            Number=pData[i];
        }
        Cnt=0;
    }

    return Number;
}

/* ���������ʽ��ֵ */
int16_t Get_Determinant_2(int16_t a11,int16_t a12,int16_t a21,int16_t a22)
{
    return (a11*a22-a21*a12);
}

/* ���Ԫһ�η��� ǰ���б������ϵ�� */
uint8_t Solve_Equations_2(int16_t(*ptr)[3],float *k,float *off)
{
    int16_t D,D1,D2;

    D=Get_Determinant_2(ptr[0][0],ptr[0][1],ptr[1][0],ptr[1][1]);

    if(D==0)
        return 1;

    D1=Get_Determinant_2(ptr[0][2],ptr[0][1],ptr[1][2],ptr[1][1]);

    D2=Get_Determinant_2(ptr[0][0],ptr[0][2],ptr[1][0],ptr[1][2]);

    *k=D1/(float)D;

    *off=D2/(float)D;

    return 0;
}

/* �����ֵ */
uint16_t Get_ABS(uint16_t x,uint16_t y)
{
    if(x>y)
        return x-y;
    else
        return y-x;
}


// �������� */
#define CCMAX   50

/* �õ������������� �����˲� ����0 ��ʾ���꿿��   �����ǲ����׵� */
uint8_t TP_GetCoorDinate(Touch_CoorDinate *TP_COOR)
{
    uint8_t bit;

    uint16_t TP_XCoor_1,TP_YCoor_1;
    uint16_t TP_XCoor_2,TP_YCoor_2;
    /* ��һ�βɼ����� */
    for(bit=0;bit<N;bit++)
    {
        /* �ɼ�10�� X ���� */
        TP_Coordinate[0][bit]=Touch_Structure.Get_XChannel();

        /* �ɼ�10�� Y ���� */
        TP_Coordinate[1][bit]=Touch_Structure.Get_YChannel();
    }

    TP_XCoor_1=TP_Data_Processing(TP_Coordinate[0],N);

    TP_YCoor_1=TP_Data_Processing(TP_Coordinate[1],N);

    /* �ڶ��βɼ����� */
    for(bit=0;bit<N;bit++)
    {
        /* �ɼ�10�� X ���� */
        TP_Coordinate[0][bit]=Touch_Structure.Get_XChannel();

        /* �ɼ�10�� Y ���� */
        TP_Coordinate[1][bit]=Touch_Structure.Get_YChannel();
    }

    TP_XCoor_2=TP_Data_Processing(TP_Coordinate[0],N);

    TP_YCoor_2=TP_Data_Processing(TP_Coordinate[1],N);

    /* ���βɼ�������֮��ܳ���CCMAX ������Ϊ���ݲ����� */

    if( Get_ABS(TP_XCoor_1 , TP_XCoor_2) >CCMAX )
        return 1;

    if( Get_ABS(TP_YCoor_1 , TP_YCoor_2) >CCMAX )
        return 1;

    /* ȡ���βɼ����ݵ�ƽ��ֵ */
    TP_COOR->TP_X=(TP_XCoor_1>>1)+(TP_XCoor_2>>1);

    TP_COOR->TP_Y=(TP_YCoor_1>>1)+(TP_YCoor_2>>1);
}

/* У׼��� */
#define CCMA        100

/* ������У׼ */
uint8_t TP_Calibration(void)
{
        Touch_CoorDinate TP_COOR[4];

    int16_t arr[2][3];

    uint16_t CC,CC1,CC2;

    /* У׼��һ���� */
    Touch_Structure.Delay_ms(500);

    Touch_Structure.DispH(Calibration_x1,Calibration_y1);

    Touch_Structure.Clear_PendingBit();

    while(1)
    {
        if(Touch_Structure.Get_StatusFlag())
        {
            Touch_Structure.Clear_PendingBit();

            if(TP_GetCoorDinate(TP_COOR))
            {
                /* ��ʾ���µ�� */
            }
            else
                break;
        }
    }


    /* У׼�ڶ����� */
    Touch_Structure.Delay_ms(500);

    Touch_Structure.DispH(Calibration_x2,Calibration_y2);

    Touch_Structure.Clear_PendingBit();

    while(1)
    {
        if(Touch_Structure.Get_StatusFlag())
        {
            Touch_Structure.Clear_PendingBit();

            if(TP_GetCoorDinate(TP_COOR+1))
            {
                /* ��ʾ���µ�� */
            }
            else
                break;
        }
    }


    /* У׼�������� */
    Touch_Structure.Delay_ms(500);

    Touch_Structure.DispH(Calibration_x3,Calibration_y3);

    Touch_Structure.Clear_PendingBit();

    while(1)
    {
        if(Touch_Structure.Get_StatusFlag())
        {
            Touch_Structure.Clear_PendingBit();

            if(TP_GetCoorDinate(TP_COOR+2))
            {
                /* ��ʾ���µ�� */
            }
            else
                break;
        }
    }


     /* У׼���ĸ��� */
    Touch_Structure.Delay_ms(500);

    Touch_Structure.DispH(Calibration_x4,Calibration_y4);

    Touch_Structure.Clear_PendingBit();

    while(1)
    {
        if(Touch_Structure.Get_StatusFlag())
        {
            Touch_Structure.Clear_PendingBit();

            if(TP_GetCoorDinate(TP_COOR+3))
            {
                /* ��ʾ���µ�� */
            }
            else
                break;
        }
    }

    /* ��������Ƿ�׼ȷ */

    CC1=Get_ABS(TP_COOR[0].TP_X , TP_COOR[3].TP_X);

    CC2=Get_ABS(TP_COOR[2].TP_X , TP_COOR[1].TP_X);

    CC=Get_ABS(CC1,CC2);

    if(CC>CCMA)
        return 1;

    CC1=Get_ABS(TP_COOR[0].TP_Y , TP_COOR[1].TP_Y);

    CC2=Get_ABS(TP_COOR[2].TP_Y , TP_COOR[3].TP_Y);

    CC=Get_ABS(CC1,CC2);

    if(CC>CCMA)
        return 1;

    /* �ⷽ���� TP_Xk TP_Yk TP_Xoffset TP_Yoffset */
    /* ����������*K+offset =Һ�������� */
    arr[0][0]=TP_COOR[0].TP_X; arr[0][1]=1; arr[0][2]=Calibration_x1;

    arr[1][0]=TP_COOR[2].TP_X; arr[1][1]=1; arr[1][2]=Calibration_x3;

    if(Solve_Equations_2(arr , &Touch_Structure.Xk , &Touch_Structure.Xoffset))
    {
        /* ��ʾ�����޽� ���ݴ��� */

        return 1;
    }

    arr[0][0]=TP_COOR[1].TP_Y; arr[0][1]=1; arr[0][2]=Calibration_y2;

    arr[1][0]=TP_COOR[3].TP_Y; arr[1][1]=1; arr[1][2]=Calibration_y4;

    if(Solve_Equations_2(arr , &Touch_Structure.Yk , &Touch_Structure.Yoffset))
    {
        /* ��ʾ�����޽� ���ݴ��� */

        return 1;
    }

        return 0;
}

/* �õ�Һ���������� ����ֵ0 ���꿿�� ���򲻿��� */
uint8_t Touch_GetCoordinate(Touch_CoorDinate *TP_COOR)
{
    if(TP_GetCoorDinate(TP_COOR))
        return 1;

    /* ����һ�㲻��õ���ֵ */
    /*
    if(TP_COOR->TP_X==0||TP_COOR->TP_X==4095 ||TP_COOR->TP_Y==0 ||TP_COOR->TP_Y==4095)
        return 1;
    */

    TP_COOR->TP_X=TP_COOR->TP_X*Touch_Structure.Xk+Touch_Structure.Xoffset;

    TP_COOR->TP_Y=TP_COOR->TP_Y*Touch_Structure.Yk+Touch_Structure.Yoffset;

    return 0;
}