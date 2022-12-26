#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"
#include <vector>
using std::vector;


class Labyrinthe : public Environnement {
private:
	int		_width;
	int		_height;
	char**	_data;
	vector<vector<char > > loadMap(char * filename);  

public:
	Labyrinthe (char*);
	int width () { return _width;}	// retourne la largeur du labyrinthe.
	int height () { return _height;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).
	void setData(int i , int j , char c){
		_data[i][j] = c;
	}
};



//结构体 Picture 用于保存 墙壁上的壁画
struct Picture
{
	char type_P;
	int x ;
	int y;
	int direction; 
};

// 结构体 Object 用来保存 box，玩家，守卫，传送门，宝藏等 位置。
struct Object
{
	int x ;
	int y;
};
// 结构SousWall用于存储 围墙的起点位置 和终点位置。
struct SousWall
{
	int x1;
	int y1;
	int x2;
	int y2;

};

int getMapWidth(vector<vector<char> > map );

vector<Picture>  getPictures(vector<vector<char> > map );
vector<Object> getObjects(char obj, vector<vector<char> > map);
vector<SousWall> getWalls(vector<vector<char> > map);

#endif
