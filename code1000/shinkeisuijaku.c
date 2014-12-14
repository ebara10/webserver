#include<stdio.h>
#include<stdlib.h>
#define MAX_LENGTH 4
#define MAX_WIDTH 13
#define MAX_CARD 52
#define SHUFFLE 20
#define ONCE_TURNED_CARD 2
#define	FINISHED_CARD 1

//カードの状態を描写する
int describe_card(int card[][MAX_WIDTH],  int information_card[][MAX_WIDTH]);
//最初のみ使用。カードをランダムに置き換える
int put_card_randam(int card[][MAX_WIDTH],int information_card[][MAX_WIDTH]);
int check_cards(int x1,int y1,int x2,int y2,int card[][MAX_WIDTH]);
int play_my_turn(int card[][MAX_WIDTH], int information_card[][MAX_WIDTH]);
void scanf_two_int(int *, int *);
int is_check_error_number(int input1, int input2,int information_card[][MAX_WIDTH]);
void draw_enemy_card(int x1,int y1,int information_card[][MAX_WIDTH]);
int main()
{
	int card_array[MAX_LENGTH][MAX_WIDTH], information_card[MAX_LENGTH][MAX_WIDTH];
	int sum = 0, my_score = 0, enemy_score = 0;

	put_card_randam(card_array, information_card);
	printf("神経衰弱をはじめましょう\n");
	while(sum <= MAX_CARD){
		my_score = play_my_turn(card_array, information_card);
		enemy_score = play_enemy_turn(card_array, information_card);	
		sum = my_score + enemy_score;
	}
	if(my_score > enemy_score){ 
		printf("貴方の勝ち\n");
	} else if(my_score == enemy_score) {
		printf("引き分け\n");
	} else {
		printf("敵の勝ち\n");
	}
	exit (0);
}

/* ----------------------------------------------------------- *
 *  play_enemy_turn:敵のターンの処理
 *  引数： 数値型の二次元配列 card[][MAX_WIDTH]:カードの番号を格納
 *  information_card[][MAX_WIDTH]:成功して既に場にないカード、一度引いたカード
 *  まだ引かれてないカードの値が格納
 *  戻り値 数値型 score得点。あたっていたら1を返す。あたっていなかったら0 
 * ----------------------------------------------------------- */
int play_enemy_turn(int card[][MAX_WIDTH], int information_card[][MAX_WIDTH]){

	int x1,y1,x2,y2,score = 0,i,j,k, error_flag = 0;
	describe_card(card, information_card);

	do{
		printf("敵のターンです\n");
		//敵のカードを引く処理
		draw_enemy_card(x1,y1, information_card);
		printf("x=%d,y=%dをめくります。\n", x1,y1);
	
		printf("2枚目に引く、座標を指定してください(x y)\n");
		//敵のカードを引く処理
		draw_enemy_card(x1,y1, information_card);
		if(x1 == x2 && y1 == y2){
			printf("エラー！xとyに同じ座標は選べません。\n");
			error_flag = -1;
			continue;
		}	
	}while(error_flag == -1);

	printf("x=%d,y=%dをめくります。\n", x2,y2);
	score = check_cards(x1, y1, x2, y2, card);
	if(score == 1){
		printf("当たりのときの処理\n");
		information_card[x1][y1] = FINISHED_CARD;
		information_card[x2][y2] = FINISHED_CARD;
		return score;
	}
	printf("外れのときの処理\n");
	int once_turned = ONCE_TURNED_CARD;
	information_card[x1][y1] = once_turned;
	information_card[x2][y2] = once_turned;

}
/* ----------------------------------------------------------- *
 *  play_my_turn:自分のターンの処理
 *  引数： 数値型の二次元配列 card[][MAX_WIDTH]:カードの番号を格納
 *  information_card[][MAX_WIDTH]:成功して既に場にないカード、一度引いたカード
 *  まだ引かれてないカードの値が格納
 *  戻り値 数値型 score得点。あたっていたら1を返す。あたっていなかったら0 
 * ----------------------------------------------------------- */
int play_my_turn(int card[][MAX_WIDTH], int information_card[][MAX_WIDTH]){

	int x1,y1,x2,y2,score = 0,i,j,k, error_flag = 0;
	describe_card(card, information_card);

	do{
		printf("貴方のターンです\n");
		printf("1枚目に引く、座標を指定してください(x y)\n");
		scanf_two_int(&x1,&y1);
		error_flag = is_check_error_number(x1,y1, information_card);
		if(error_flag != 0){
			continue;
		}
		printf("x=%d,y=%dをめくります。\n", x1,y1);
	
		printf("2枚目に引く、座標を指定してください(x y)\n");
		scanf_two_int(&x2,&y2);
		error_flag = is_check_error_number(x2,y2,information_card);
		if(x1 == x2 && y1 == y2){
			printf("エラー！xとyに同じ座標は選べません。\n");
			error_flag = -1;
			continue;
		}	
	}while(error_flag == -1);

	printf("x=%d,y=%dをめくります。\n", x2,y2);
	score = check_cards(x1, y1, x2, y2, card);
	if(score == 1){
		printf("当たりのときの処理\n");
		information_card[x1][y1] = FINISHED_CARD;
		information_card[x2][y2] = FINISHED_CARD;
		return score;
	}
	printf("外れのときの処理\n");
	int once_turned = ONCE_TURNED_CARD;
	information_card[x1][y1] = once_turned;
	information_card[x2][y2] = once_turned;
	return score;
}


void draw_enemy_card(int x,int y,int information_card[][MAX_WIDTH])
{
	int error_flag = 0;
	do{
		x = rand() % ((MAX_LENGTH -1) - 0 + 1) + 0;
		y = rand() % ((MAX_WIDTH -1) - 0 + 1) + 0;
		printf("draw_enemy_card x=%d,y=%d\n",x,y);
		error_flag = is_checked_overlap(information_card);
	}while(error_flag != 0);

}
/* ----------------------------------------------------------- *
 *  is_check_error_number:引いたカードのエラーチェック。
 *  チェック内容→ 引いたカードは範囲内か、既にあたりで場にないものはないか
 *  引数：input1,input2:x座標とy座標, information_card[][MAX_WIDTH]:カード情報
 *  戻り値　error_flag エラーがあった場合-1
 * ----------------------------------------------------------- */
int is_check_error_number(int input1, int input2, int information_card[][MAX_WIDTH])
{
	int error_flag = 0, i,j;
	printf("is_check_error_numberの中input1=%d,input2=%d", input1, input2);
	if(input1 > MAX_LENGTH){
		printf("エラー！xには%dより小さい値を入れて下さい\n",MAX_LENGTH);
		error_flag = -1;
	}
	if(input2 > MAX_WIDTH){
		printf("エラー！yには%dより小さい値を入れてください。\n", MAX_WIDTH);
		error_flag = -1;
	} 
	error_flag = is_checked_overlap(information_card);
	if(error_flag != 0){
		printf("エラー！指定された場所のカードは既に選択済みです\n");
	}
	return error_flag;
}

int is_checked_overlap(int information_card[][MAX_WIDTH])
{
	int i,j, error_flag = 0,finished = FINISHED_CARD;
	//カードの重複チェック
	for(i = 0; i < 4; i++){
		for(j = 0; j < 13; j++){
			if(information_card[i][j] == finished){
				error_flag == -1;
			}
		}
	} 
	return error_flag;

}

/* ----------------------------------------------------------- *
 *  scanf_two_int:2つの数値型の入力を受け付ける関数。数値以外のものが
 *  入力されるとバッファをクリアして、正常な値を読み込むまで繰り返してくれる
 *  引数： なし
 *  戻り値 数値型 input:ユーザが入力した内容
 * ----------------------------------------------------------- */

void scanf_two_int(int *input1, int *input2)
{
    while(1){
        if(scanf("%d %d", input1, input2) != 2){
            printf("エラー！数値以外のものが入力されました%d,%d\n", *input1, *input2);
            scanf("%*s");
            continue;
        }
        break;
    }
}

int check_cards(int x1,int y1,int x2,int y2,int card[][MAX_WIDTH])
{
	printf("めくったカードは%dと%dでした\n", card[x1][y1], card[x2][y2]);
	if(card[x1][y1] == card[x2][y2]){
		printf("あたりです\n");
		return 1;
	}
	printf("はずれ\n");
	return 0;

}

int describe_card(int card[][MAX_WIDTH],int information_card[][MAX_WIDTH])
{
	int i = 0, j = 0;
	printf("■ まだめくっていない\n");
	printf("X 合ってた！\n");
	printf("□ 一回だけめくっている\n");

printf("information\n");
	for(i = 0; i < 4; i++){
		printf("%d ", i);
		for(j = 0; j < 13; j++){
			printf("%d,", information_card[i][j]);
		}
	printf("\n");
	} 
	printf("x y →\n");
	printf("↓");
	for(j = 0;j < 13;j++){
		printf("%2d ",j);
	}
	printf("\n");
	printf("  |-----------------------------------|\n");

	for(i = 0; i < 4; i++){
		printf("%d ", i);
		for(j = 0; j < 13; j++){
			if(information_card[i][j] == FINISHED_CARD){
				printf("X  ");
				continue;
			}
			if(information_card[i][j] == ONCE_TURNED_CARD){
				printf("□  ");
				continue;
			}
			printf("■  ");
		}
		printf("\n");
	}

}
int put_card_randam(int card[][MAX_WIDTH],int information_card[][MAX_WIDTH])
{
	int i = 0, j = 0, x1,x2,y1,y2, buffer;
	printf("put_card_randamの中\n");
	for(i = 0; i < 4; i++){
		for(j = 0; j < 13; j++){
			card[i][j] = j+1;
			information_card[i][j] = 0;
printf("%d",  information_card[i][j]);
		}
		printf("\n");
	}
	srand(time(0));
	
	int tmp, a,b;
	for(i = 0;i < MAX_LENGTH; i++){
		for(j = 0;j < MAX_WIDTH; j++){
			a = rand()%MAX_WIDTH;
			b = i % MAX_WIDTH;
			tmp = card[i][a];
			card[i][a] = card[i][b];
			card[i][b] = tmp;
		}
	}

	printf("シャッフル後\n");
	for(i = 0; i < 4; i++){
		for(j = 0; j < 13; j++){
			printf("%d ",  card[i][j]);
		}
		printf("\n");
	}
	
}

int is_error_check()
{
	
return 0;	
}
