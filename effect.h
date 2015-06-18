//=============================================================================
//
// effect.h
// �G�t�F�N�g�֘A�e�폈��
// GP12A331 ���V���琶
//
// 6/4���݂͊g�k�̃G�t�F�N�g�݂̂Ȃ̂ŁA�A�b�v�f�[�g�ȊO���ɂȂ�
// �e�t�����better	
//
//=============================================================================
#ifndef _EFFECT__
#define _EFFECT__

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
//#include "playbar.h"


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int CunterEffect;					// �g�k�̃J�E���^�[�@���X�Ɋg�k���邽�߂Ɏg�p
	int CounterSpeed;					// �g�k�̑���
	int CountRevers;					// �g�k��+-���]�p
	float fWidth;						// ���݂̕�
	float fHeight;						// ���݂̍���
	float fBeforeWidth;					// ���̕�   �ۑ��p���[�N
	float fBeforeHeight;				// ���̍��� �ۑ��p���[�N
	bool bUse;							// �G�t�F�N�g���g�p����
}EFFECT;

void InitBarEffect(void);
void UninitBarEffect(void);
//void UpdateBarEffect(PLAYBAR *playbar);
void DrawBarEffect(void);
//void SetBarEffect(PLAYBAR *pPlaybar);
void SetBarEffect(int nPlaybarNum);

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#endif