#include "DxLib.h"
#include "MenuScene.h"
#include "SelectScene.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

MenuScene::MenuScene() {
	description = false;
	page = 0;
	cursorPos = 0;
	animationCount = 0;
}

void MenuScene::init() {

}

Scene* MenuScene::update() {
	if ( description ) {
		if ( KeyBoard::key[KEY_INPUT_LEFT] == 1 ) {
			page -= 1;
			if ( page < 0 ) page = 0;
		}
		if ( KeyBoard::key[KEY_INPUT_RIGHT] == 1 ) {
			page += 1;
			if ( page > 1 ) page = 1;
		}
		if ( KeyBoard::key[KEY_INPUT_RETURN] == 1 ) {
			description = false;
			page = 0;
		}
	} else {
		if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
			cursorPos -= 1;
			if ( cursorPos < 0 ) cursorPos = 1; // �ő�l����
		}
		if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) cursorPos = ( cursorPos + 1 ) % 2;

		if ( KeyBoard::key[KEY_INPUT_RETURN] == 1 ) {
			if ( cursorPos == 1 ) description = true;
			else if ( cursorPos == 0 ) {
				SelectScene* sScene = new SelectScene();
				return sScene;
			}
		}
	}
	return this;
}

void MenuScene::draw() {
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 0, Loader::imageHandles[Loader::BLOCK_1], TRUE );
	for ( int y = 1; y < 14; y++ ) {
		DrawGraph( 0, y * 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );
		DrawGraph( 640 - 32, y * 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );
	}
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 480 - 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );

	if ( description ) {
		// ������`��
		if ( page == 0 ) {
			DrawStringToHandle( 50, 50, "�u���b�N���󂵂āC��ɓ��ꂽ�u���b�N�œ�����낤�I", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 50, 80, "���q�̃|�b�L������ƈꏏ�ɏo����ڎw���񂾁I", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 130, Loader::imageHandles[Loader::RIGHT], TRUE );
			DrawStringToHandle( 96, 136, "�|�b�L������F�T������D���C�������q�ɂȂ�", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 190, Loader::imageHandles[Loader::BLOCK_1_CRACKED], TRUE );
			DrawStringToHandle( 96, 194, "�u���b�N1�F�ꌂ�ł����Ă��܂��قǂ��낢", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 240, Loader::imageHandles[Loader::BLOCK_2_CRACKED], TRUE );
			DrawStringToHandle( 96, 244, "�u���b�N2�F������Ɗ��A�Ȃ��Ȃ������Ȃ�", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 290, Loader::imageHandles[Loader::SPRING_BLOCK], TRUE );
			DrawStringToHandle( 96, 294, "�u���b�N3�F�e�͂������܂����C�Ƃׂ邩���H", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 40, 340, Loader::imageHandles[Loader::BLOCK_1], TRUE );
			DrawGraph( 72, 340, Loader::imageHandles[Loader::BLOCK_2], TRUE );
			DrawGraph( 40, 372, Loader::imageHandles[Loader::DISAPPEARING_BLOCK], TRUE );
			DrawGraph( 72, 372, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
			DrawStringToHandle( 114, 360, "�󂹂Ȃ��u���b�N�F���̖��̂Ƃ���ł���", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "1/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 450, 415, "Enter�F����", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawTriangleAA( 640 - 45, 480 / 2 - 15, 640 - 45, 480 / 2 + 15, 640 - 20, 480 / 2, GetColor( 255, 255, 255 ), TRUE );
		} else if ( page == 1 ) {
			DrawStringToHandle( 50, 45, "�������", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 60, 80, "�� or ���F�ړ�", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 110, "Space�F�W�����v", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 140, "C +�i���L�[�j�F�p���`�i8�����j", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 170, "Z +�i���L�[�j�F�u���b�N��u���i8�����j", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 200, "S or D�F�u�������u���b�N�̑I��", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 230, "Esc�F�|�[�Y��ʁi��蒼���C�V�ѕ������j", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 270, "�u���b�N�ւ̍U���ƃu���b�N�̔z�u�͍�shift��", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 55, 300, "�ꏏ�ɉ����Ă����Ƃ��̏�Ŏ~�܂����܂܍s����", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 340, "���u���b�N3�ȊO�̃u���b�N�́C��ɓ���Ă���", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 55, 370, "�@�u���Ă��܂��Ƃ���ȍ~���킹�Ȃ��Ȃ��I", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "2/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 450, 415, "Enter�F����", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawTriangleAA( 45, 480 / 2 - 15, 45, 480 / 2 + 15, 20, 480 / 2, GetColor( 255, 255, 255 ), TRUE );
		}
	} else {
		int standardX, standardY;
		if ( cursorPos == 0 ) {
			standardX = 400;
			standardY = 100 - 1;
		} else {
			standardX = 400;
			standardY = 200 - 1;
		}
		DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		
		DrawStringToHandle( 75 + 75, 200, "�V�ѕ�", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 75, 100, "�X�e�[�W�Z���N�g", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 165, 360, "Press Enter Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

		animationCount = ( animationCount + 1 ) % 24;
	}
}