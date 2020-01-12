#include "KeyBoard.h"

char KeyBoard::key[256];

int KeyBoard::updateKey() {
    char tmpKey[256]; // 現在のキーの入力状態を格納する
    GetHitKeyStateAll( tmpKey ); // 全てのキーの入力状態を得る
    for ( int i = 0; i < 256; i++ ) {
        if ( key[i] >= CHAR_MAX ) key[i] = 1;
        if ( tmpKey[i] != 0 ) { // i番のキーコードに対応するキーが押されていたら
            key[i]++;     // 加算
        } else { // 押されていなければ
            key[i] = 0;
        }
    }
    return 0;
}