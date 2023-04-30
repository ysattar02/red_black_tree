 for (int i = 0; i < 3; i++){
            if (board[i][0] == "X" && board[i][1] == "X" && board[i][2] == "X"){
                cout << "Player 1 Won" << endl;
                game = false;
            }
            if (board[0][i] == "X" && board[1][i] == "X" && board[2][i] == "X"){
                cout << "Player 1 Won" << endl;
                game = false;
            }
        }
        if (board[0][0] == "X" && board[1][1] == "X" && board[2][2] == "X"){
            cout << "Player 1 Won" << endl;
            game = false;
        }
        if (board[2][0] == "X" && board[1][1] == "X" && board[0][2] == "X"){
            cout << "Player 1 Won" << endl;
            game = false;
        }
        for (int i = 0; i < 3; i++){
            if (board[i][0] == "O" && board[i][1] == "O" && board[i][2] == "O"){
                cout << "Player 2 Won" << endl;
                game = false;
            }
            if (board[0][i] == "O" && board[1][i] == "O" && board[2][i] == "O"){
                cout << "Player 2 Won" << endl;
                game = false;
            }
        }
        if (board[0][0] == "O" && board[1][1] == "O" && board[2][2] == "O"){
            cout << "Player 2 Won" << endl;
            game = false;
        }
        if (board[2][0] == "O" && board[1][1] == "O" && board[0][2] == "O"){
            cout << "Player 2 Won" << endl;
            game = false;
        }
