 /* ͷ�ļ� touch.h */

#ifndef TOUCH_H
#define TOUCH_H

/* ��������ļ� */
//#include "Diver.h"

typedef unsigned short int uint16_t ;

typedef unsigned char uint8_t;

typedef short int int16_t;

/* ��������Ҫ���ݽṹ�� */
typedef struct
{
    /* �õ�������Xͨ��AD���ݵķ��� */
    uint16_t (* const Get_XChannel)(void);

    /* �õ�������Yͨ��AD���ݵķ��� */
    uint16_t (* const Get_YChannel)(void);

    /* ��ʱ���� */
    void (*Delay_ms)(uint16_t);

    /* �鿴�������Ƿ񱻰���  ����0��ʾδ���� ���򱻰��� */
    uint8_t (*Get_StatusFlag)(void);

    void (*Clear_PendingBit)(void);

    /* ��Һ��������ʾʮ�ֺź��� */
    void (*DispH)(uint16_t x ,uint16_t y);

    /* Һ������X=��������X*TP_Xk+TP_Xoffset Y����Ҳͬ��*/
    float Xk;

    float Yk;

    float Xoffset;

    float Yoffset;

}Tuch_TypeDef;

/* ����ṹ�� */
typedef struct
{
    uint16_t TP_X;

    uint16_t TP_Y;

}Touch_CoorDinate;


#define Calibration_x1  50
#define Calibration_y1  50

#define Calibration_x2  50
#define Calibration_y2  190

#define Calibration_x3  270
#define Calibration_y3  190

#define Calibration_x4  270
#define Calibration_y4  50


/* �õ�Һ���������� ����ֵ0 ���꿿�� ���򲻿��� */
uint8_t Touch_GetCoordinate(Touch_CoorDinate *TP_COOR);

/* ������У׼ ����0 У׼�ɹ�  ����ʧ�� */
uint8_t TP_Calibration(void);



#endif // TOUCH_H_INCLUDED