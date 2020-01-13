#include "DxLib.h"
#include "MenuScene.h"
#include "SelectScene.h"
#include "KeyBoard.h"
#include "Loader.h"

MenuScene::MenuScene() {
	font[0] = CreateFontToHandle( NULL, 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
	font[1] = CreateFontToHandle( NULL, 36, 3, DX_FONTTYPE_ANTIALIASING_EDGE );

	description = false;
	cursorPos = 0;
	animationCount = 0;
}

void MenuScene::init() {

}

Scene* MenuScene::update() {
	if ( description ) {
		if ( KeyBoard::key[KEY_INPUT_ESCAPE] == 1 && description ) {
			description = false;
		}
	} else {
		if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 ) {
			if ( cursorPos == 0 ) description = true;
			else if ( cursorPos == 1 ) {
				SelectScene* sScene = new SelectScene();
				return sScene;
			}
		}
		if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
			cursorPos -= 1;
			if ( cursorPos < 0 ) cursorPos = 1; // �ő�l����
		}
		if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) cursorPos = ( cursorPos + 1 ) % 2;
	}
	return this;
}

void MenuScene::draw() {

	if ( description ) {
		// ������`��
		DrawStringToHandle( 40, 40, "�u���b�N���󂵂Ď�ɓ���悤�I", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 70, "��ɓ��ꂽ�u���b�N�œ�������ăS�[����ڎw�����I", GetColor( 255, 255, 255 ), font[0] );

		DrawStringToHandle( 40, 130, "�� or ���F�ړ�", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 160, "Space�F�W�����v", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 190, "C + (��or��)�F�U���i�㉺���E�j", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 220, "Z + (��or��)�F�u���b�N��u���i�㉺���E�j", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 250, "S or D�F�u���b�N�̑I��", GetColor( 255, 255, 255 ), font[0] );

		DrawStringToHandle( 40, 310, "�u���b�N�ւ̍U���ƃu���b�N�̔z�u��", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 340, "��shift + ���L�[ �����킹�邱�Ƃ�", GetColor( 255, 255, 255 ), font[0] );
		DrawStringToHandle( 40, 370, "�΂ߕ����ɂ��Ή��\", GetColor( 255, 255, 255 ), font[0] );

		DrawStringToHandle( 380, 415, "Esc�F���j���[�ɖ߂�", GetColor( 255, 255, 255 ), font[0] );
	} else {
		int standardX, standardY;
		if ( cursorPos == 0 ) {
			standardX = 400;
			standardY = 100 - 1;
		} else {
			standardX = 400;
			standardY = 200 - 1;
		}
		DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		
		DrawStringToHandle( 60, 100, "�V�ѕ�", GetColor( 255, 255, 255 ), font[1] );

		DrawStringToHandle( 60, 200, "�X�e�[�W�Z���N�g", GetColor( 255, 255, 255 ), font[1] );

		DrawStringToHandle( 160, 360, "Press Space Key", GetColor( 255, 255, 255 ), font[1] );

		animationCount = ( animationCount + 1 ) % 24;
	}

	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 0, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
	for ( int y = 1; y < 14; y++ ) {
		DrawGraph( 0, y * 32, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
		DrawGraph( 640 - 32, y * 32, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
	}
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 480 - 32, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
}