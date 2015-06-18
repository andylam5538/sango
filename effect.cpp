//=============================================================================
//
// effect.cpp
// �G�t�F�N�g�֘A�e�폈��
// GP12A331 ���V���琶
//
// 6/4���݂͊g�k�̃G�t�F�N�g�݂̂Ȃ̂ŁA�A�b�v�f�[�g�ȊO���ɂȂ�
// �e�t�����better	
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "effect.h"
#include "playbar.h"
#include "input.h"
#include "gameManager.h"
#include "timeTrack.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_UPPER_MAX	(120)
#define EFFECT_LOWWER_MAX	(80)
#define COUNT_SPEED			(2)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
EFFECT		g_BarEffect;
PLAYBAR*	g_pPlaybar_Eff = GetPlaybar();
//=============================================================================
//
// void InitBarEffect(void)
// Playbar�̃G�t�F�N�g�̏���������
// 
//
//=============================================================================
void InitBarEffect(void)
{
	g_BarEffect.CunterEffect = 0;				// �g�k�̃J�E���^�[�@���X�Ɋg�k���邽�߂Ɏg�p
	g_BarEffect.CounterSpeed = COUNT_SPEED;		// �g�k�̑���
	g_BarEffect.CountRevers = 1;				// 1��-1������@�g�k��+-�Ɏg�p
	g_BarEffect.fWidth = 0;						// ���݂̕�
	g_BarEffect.fHeight = 0;					// ���݂̍���
	g_BarEffect.fBeforeWidth = 0;				// ���̕�   �ۑ��p���[�N
	g_BarEffect.fBeforeHeight = 0;				// ���̍��� �ۑ��p���[�N
	g_BarEffect.bUse = false;					// �g�p����
}

//=============================================================================
//
// void UninitBarEffect()
// Playbar�̃G�t�F�N�g�̏I������
// ���ɂȂ�
//
//=============================================================================
void UninitBarEffect(void)
{
	
}

//=============================================================================
//
// void UpdateBarEffect(void)
// Playbar�̃G�t�F�N�g�̃A�b�v�f�[�g����
// ����PlayBar�̃|�C���^
// SetBarEffect()�����
//
//=============================================================================
void UpdateBarEffect(PLAYBAR *playbar)
{
	// �G�t�F�N�g���ł����
	if(g_BarEffect.bUse)
	{
		// ���N���b�N���Ă���Ƃ�
		if(IsMouseLeftPressed())
		{	
			// �J�E���^�[���
			if(g_BarEffect.CunterEffect >= EFFECT_UPPER_MAX)
			{
				// ���ɂȂ�
				g_BarEffect.CunterEffect = EFFECT_UPPER_MAX;
			}
			// �J�E���^�[����
			else if(g_BarEffect.CunterEffect <= EFFECT_LOWWER_MAX)
			{
				g_BarEffect.CountRevers *= -1;					// �����܂ōs�����̂ŁA�J�E���g���]
				g_BarEffect.CunterEffect = EFFECT_LOWWER_MAX;	// �ꉞ�␳
			}
			// �J�E���^�[�͈͓��Ȃ�
			else
			{
				g_BarEffect.CunterEffect += g_BarEffect.CounterSpeed * g_BarEffect.CountRevers;	// �J�E���^�[���Z
			}
		}
		// ���N���b�N����Ă��Ȃ���
		else
		{
			playbar->fHeight = g_BarEffect.fBeforeHeight;	// �g�k�����O��width��ۑ�
			playbar->fWidth = g_BarEffect.fBeforeWidth;		// �g�k�����O��height��ۑ� 
			InitBarEffect();								// ���N���b�N�𗣂��ꂽ�̂ŁA������
		}

		playbar->fHeight = g_BarEffect.fHeight;				// �v�Z��c�����
		playbar->fWidth = g_BarEffect.fWidth;				// �v�Z�㉡�����
	}
}

//=============================================================================
//
// void DrawBarEffect()
// Playbar�̃G�t�F�N�g�̕`�揈��
// �g�k�݂̂Ȃ̂œ��ɂȂ�
//
//=============================================================================
void DrawBarEffect(void)
{
	
}

//=============================================================================
//
// void SetBarEffect()
// �G�t�F�N�g�Z�b�g
// BeforeWidth��BeforeHeight��ۑ��EbUse��true
//
//=============================================================================
//void SetBarEffect(PLAYBAR *pPlaybar)
//{
//	// �g�k�O�̏����i�[
//	g_BarEffect.fBeforeHeight = pPlaybar->fHeight;
//	g_BarEffect.fBeforeWidth = pPlaybar->fWidth;
//	
//	// �g�k�p�ϐ�
//	g_BarEffect.fHeight = pPlaybar->fHeight;
//	g_BarEffect.fWidth = pPlaybar->fWidth;
//	g_BarEffect.bUse = true;
//}

void SetBarEffect(int nPlaybarNum)
{
	// �g�k�O�̏����i�[
	g_BarEffect.fBeforeHeight = g_pPlaybar_Eff[nPlaybarNum].fHeight;
	g_BarEffect.fBeforeWidth = g_pPlaybar_Eff[nPlaybarNum].fWidth;
	
	// �g�k�p�ϐ�
	g_BarEffect.fHeight = g_pPlaybar_Eff[nPlaybarNum].fHeight;
	g_BarEffect.fWidth = g_pPlaybar_Eff[nPlaybarNum].fWidth;
	g_BarEffect.bUse = true;
}

void MousePressedEffect(int nPlaybarNum)
{

}