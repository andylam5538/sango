/*****************************************************************************
 *
 * �f�o�b�O�\������ [debugproc.h]
 *
 * ��鎞�� �F 2015.5.1
 *
 * GP11A 248 ���m 40582
 *
 ******************************************************************************/
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);

void PrintDebugProc(char *fmt,...);

#endif