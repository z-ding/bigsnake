#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;

bool gameover = false;
int point = 0;
int ttl_fruits = 0;

void input() {
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameover = true;
			break;
		}
	}
}
class map {
private:
	int width;
	int height;
public:
	void setmap(int x, int y) {
		width = x;
		height = y;
	}
	vector<int> getmap() {
		vector<int> v;
		v.push_back(width);
		v.push_back(height);
		return v;
	}
	vector<vector<string>> createmap() {
		vector<vector<string>> initialmap;
		vector<string> m;
		for (int i = 0; i <= width; i++) {
			m.push_back("#"); //top row of the map
		}
		initialmap.push_back(m);
		for (int i = 1; i < height; i++) { // row 2 to second last of the map
			vector<string> n;
			n.push_back("#");
			for (int j = 1; j < width; j++) {
				n.push_back(" ");
			}
			n.push_back("#");
			initialmap.push_back(n);
		}

		vector<string> p;
		for (int i = 0; i <= width; i++) {
			p.push_back("#"); //top row of the map
		}
		initialmap.push_back(p);
		return initialmap;
		
	}
};

class snake {
private:
	int head_x;
	int head_y;
	vector<pair<int, int>> snakebody;
	int bodylen;//the length of snake's body (excluding head)
public:
	void setsnakehead(int x, int y) {
		head_x = x;
		head_y = y;
		
	}
	vector<int> getsnakehead() {
		vector<int> v;
		v.push_back(head_x);
		v.push_back(head_y);
		return v;
	}

	void setlength(int x) {
		bodylen = x;
	}

	void addlength() {
		bodylen ++;
	}

	int getlength() {
		return bodylen;
	}

	void movesnakehead(vector<vector<string>>& map) {

		map[head_x][head_y] = " "; //clear previous head location
		int prev_x = head_x;
		int prev_y = head_y;
		bool lenadd = false;
		switch (dir)
		{
		case LEFT:
			head_y--;
			break;
		case RIGHT:
			head_y++;
			break;
		case UP:
			head_x--;
			break;
		case DOWN:
			head_x++;
			break;
		default:
			break;
		}

		if (head_x ==0 || head_x == map.size()-1|| head_y ==0 || head_y == map[0].size()-1 || map[head_x][head_y] == "o") {
			gameover = true;
			cout << "gameover" << endl;
			
		}

		if (map[head_x][head_y] == "+") { //snakehead meet a add1fruit
			bodylen++;
			ttl_fruits--;
			point++;
			lenadd = true;
			auto p = make_pair(prev_x, prev_y); //add one more node to the snake body
			snakebody.push_back(p);
		}
		else if (map[head_x][head_y] == "-") { //snakehead meet a minus1fruit
			if (bodylen == 0) {
				gameover = true;
				cout << "gameover" << endl;
			}
			else {
				bodylen--;
				map[snakebody[snakebody.size()-1].first][snakebody[snakebody.size() - 1].second] = " ";
				snakebody.pop_back();
				ttl_fruits--;
				point += 2;			
			}
		}
		else if (map[head_x][head_y] == "*") { //snakehead meet a rotatefruit
			ttl_fruits--;
			point+=5;
			int r = rand() % 4;
			switch (r)
			{
			case 0:
				if (dir != RIGHT) {
					dir = LEFT;
				}
				break;
			case 1:
				if (dir !=LEFT) {
					dir = RIGHT;
				}
				break;
			case 2:
				if (dir != DOWN) {
					dir = UP;
				}
				break;
			case 3:
				if (dir != UP) {
					dir = DOWN;
				}
				break;
			}

		}

		map[head_x][head_y] = "O";


		//move the body of snake following the snake head
		if (bodylen >0) {
			for (int i = 0; i<snakebody.size();i++) {
				int next_x = snakebody[i].first;
				int next_y = snakebody[i].second;
				snakebody[i].first = prev_x;
				snakebody[i].second = prev_y;
				prev_x = next_x;
				prev_y = next_y;
			}

			for (int i = 0; i < snakebody.size(); i++) {
				map[snakebody[i].first][snakebody[i].second] = "o";
			}

			if (lenadd == false) {
				map[prev_x][prev_y] = " ";//clear the last position of snake tail after movement
			}
		}


	}

	void draw(vector<vector<string>>& map) {
		system("cls"); //clear screen
		cout << "+: add snake length by 1 -- point +=1" << endl;
		cout << "-: deduct snake length by 1 -- point +=2" << endl;
		cout << "*: change the snake direction randomly -- point +=5" << endl;
		cout << "ttl points " << point << endl;
		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map[i].size(); j++) {
				cout << map[i][j] << " ";
			}
			cout << endl;
		}
		
		
	}
};

class fruits {
protected:
	int fruit_x=0;
	int fruit_y=0;
	int bonus; //points added / deducted for each fruits
public:
	virtual ~fruits() {
		//virtual destructor for base class
		cout << " base class virtual destructor" << endl;
	}
	virtual void generatefruit(vector<vector<string>>& map) { //setter for the fruit location
		cout << " base class virtual function" << endl;
	}
	virtual void setbonus() {
		
	}
	virtual int getbonus() {
		cout << " base class virtual function" << endl;
		return bonus;
	}

};

class add1fruit : public fruits{
	~add1fruit() {
		//destructor of derived class
	}
	void setbonus() {

	}
	int getbonus() {
		return bonus;
	}

	void generatefruit(vector<vector<string>>& map) { //setter for the fruit location
		while (map[fruit_x][fruit_y] != " " && map[fruit_x][fruit_y] != "o") { //fruit can only be placed in empty place or snake body
			fruit_x = rand() % (map.size() - 2) + 1;
			fruit_y = rand() % (map[0].size() - 2) + 1;
		}
		map[fruit_x][fruit_y] = "+";
		ttl_fruits++;
	}

};

class minus1fruit : public fruits {
	~minus1fruit() {
		//destructor of derived class
	}
	void setbonus() {

	}
	int getbonus() {
		return bonus;
	}

	void generatefruit(vector<vector<string>>& map) { //setter for the fruit location
		while (map[fruit_x][fruit_y] != " " && map[fruit_x][fruit_y] != "o") {
			fruit_x = rand() % (map.size() - 2) + 1;
			fruit_y = rand() % (map[0].size() - 2) + 1;
		}
		map[fruit_x][fruit_y] = "-";
		ttl_fruits++;
	}

};

class rotatefruit : public fruits {
	~rotatefruit() {
		//destructor of derived class
	}
	void setbonus() {

	}
	int getbonus() {
		return bonus;
	}

	void generatefruit(vector<vector<string>>& map) { //setter for the fruit location
		while (map[fruit_x][fruit_y] != " " && map[fruit_x][fruit_y] != "o") {
			fruit_x = rand() % (map.size() - 2) + 1;
			fruit_y = rand() % (map[0].size() - 2) + 1;
		}
		map[fruit_x][fruit_y] = "*";
		ttl_fruits++;
	}

};

int main() {
	srand(time(NULL));
	map m1;
	m1.setmap(20, 20);
	vector<vector<string>> m1_vec;
	m1_vec = m1.createmap();

	snake s1;
	s1.setsnakehead(m1.getmap()[0] / 2, m1.getmap()[1] / 2);
	s1.setlength(0) ;
	fruits* f;
	
	while (! gameover) {
		s1.draw(m1_vec);
		
		if (ttl_fruits<5) {
			int random = rand() % 10;
			if (random <2) { //chance 20%
				f = new minus1fruit(); // for further fruit built
			}
			else if (random ==9) { //chance 10%
				f = new rotatefruit();
			}
			else { //chance 70%
				f = new add1fruit();
			}
			f->generatefruit(m1_vec);
		}
		input();
		s1.movesnakehead(m1_vec);
		
		Sleep (200);
	}
	
	return 0;
}