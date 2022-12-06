	#define EMPTY 0
	#define BLACK 1
	#define WHITE 2
uint32_t gameUpdate(int32_t x, int32_t y){
	static int32_t nMok=-1, i, j, countScore;
	static int8_t userTurn=BLACK;
	static int8_t **board=NULL;
	static POINT pos, countPos, boardSize={-1,-1};

	static auto checkSettings = [&]()->bool{
		if(nMok>0 && boardSize.x>0 && boardSize.y>0){
			board = (int8_t **)malloc(sizeof(int8_t *) * (boardSize.x+1));
			for(i = 0;i <= boardSize.x; i++)board[i] = (int8_t *)calloc((boardSize.y+1), sizeof(int8_t));
			return true;
		}else return false;
	};

	if(x<0){ //Commands
		switch(x){
			case -1:{	//Set nMok
				nMok=y;
				return 0x01 | checkSettings()<<3;
			}
			case -2:{
				boardSize.x=y;
				return 0x02 | checkSettings()<<3;
			}
			case -3:{
				boardSize.y=y;
				return 0x04 | checkSettings()<<3;
			}
			case -4:{
				if(board){
					for(i=boardSize.x;i+1;i--)free(board[i]);
					free(board);
					return 1; 
				}else return 0;
			}
		}
	}
	else{ //Run
		if(board[x][y])return 0;
		board[x][y]=userTurn;
    	for(i = 0;i<4;i++){		//돌 검사
			countScore = 0;
			pos = {x,y};

			switch(i){
				case 0: countPos.x = 1;	//가로
						countPos.y = 0;
						pos.x -= (nMok - 1);
						break;
				case 1: countPos.x = 1;	//우하향
						countPos.y = 1;
						pos.x -= (nMok - 1);
						pos.y -= (nMok - 1);
						break;
				case 2: countPos.x = 0;	//세로
						countPos.y = 1;
						pos.y -= (nMok - 1);
						break;
				case 3: countPos.x = 1;	//우상향
						countPos.y = -1;
						pos.x -= (nMok - 1);
						pos.y += (nMok - 1);
			}

			for(j = 0;j < 2 * nMok-1;j++){	//돌 갯수 세는 부분
				if((pos.x<1||pos.x>boardSize.x)||(pos.y<1||pos.y>boardSize.y)){
					pos.x += countPos.x;
					pos.y += countPos.y;
					continue;
				}
				else if(board[pos.x][pos.y]==userTurn)
					countScore++;
				else
					countScore = 0;

				if(countScore>=nMok){	//승리할 경우 메세지 출력 후 종료
					switch(userTurn){
						case BLACK:return 0x1 | 0x2;//Black
						case WHITE:return 0x1 | 0x4;//White
					}
				}

				pos.x += countPos.x;
				pos.y += countPos.y;
			}
		}

		switch(userTurn){	//흑백 변경
			case BLACK: userTurn = WHITE;break;
			case WHITE: userTurn = BLACK;break;
		}
		return 0x1;
	}
}
