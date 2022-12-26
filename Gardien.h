#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include <math.h>
#include <iostream>
#include <ctime>
using std::cout;
using std::endl;
using std::min;
using std::max;
using std::abs;

const double PI = 3.1415926535;

class Labyrinthe;


class Gardien : public Mover {
public:
	int speed_shoot; // 守卫 射速
	int collision_time ; // 冲撞时间
	double move_ds ; // 守卫移动速度
	Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
	{
		speed_shoot = 0;
		collision_time = 0;
		move_ds = 0.1;

	}

	// mon gardien pense tr�s mal!
	void update (void) {
		
	//如果守卫冲撞 墙壁3次则 守卫反向移动。
		if(collision_time == 3){
			move_ds = 0 - move_ds;
			collision_time = 0;
		}
			
		 

		//判断守卫是否冲撞墙壁。
		if(!move(move_ds,move_ds))
			collision_time ++;
	

		int x_player = _l->_guards[0]->_x;
		int y_player = _l->_guards[0]->_y;
		int x_enemy = _x;
		int y_enemy = _y;
		//计算玩家和守卫的距离
		float dis_y = (float)abs(y_player - y_enemy);
		float dis_x = (float)abs(x_player - x_enemy);
		float dis_total = dis_y*dis_y + dis_x*dis_x ;

		//玩家等待时间-1， 如果玩家等待时间为负数 则让它保持为0；
		((Chasseur*)_l->_guards[0])->wait_time --;
	
		if (((Chasseur*)_l->_guards[0])->wait_time < 0)
		{
			((Chasseur*)_l->_guards[0])->wait_time = 0;
		}
		//玩家等待时间为0，且hp不等于100，则开始恢复血量。
		if(((Chasseur*)_l->_guards[0])->wait_time == 0 && ((Chasseur*)_l->_guards[0])->hp != 100){
			((Chasseur*)_l->_guards[0])->hp ++;
			message("Hunter HP : %d",((Chasseur*)_l->_guards[0])->hp);
		}
// 如果玩家和守卫距离<= 25000 且 中间没有墙。则会被敌人发现并开火。
	if(dis_total <= 25000 && !existWall()){
		speed_shoot ++;
		int angle = (int)(atan(dis_y/dis_x)*180/PI) ;
		//敌人转向
		if (y_enemy == y_player && x_player > x_enemy)
			_angle = 270;
		else if (y_enemy == y_player && x_player < x_enemy)
			_angle = 90;
		else if (x_player == x_enemy && y_player > y_enemy)
			_angle = 0;
		else if (x_player == x_enemy && y_player < y_enemy)
			_angle = 180;
		//右下角
		else if (x_player > x_enemy &&  y_player < y_enemy)
		 	_angle = 270 -angle;
		//左下角
		else if (x_player < x_enemy && y_player < y_enemy)
			_angle = 90+angle;
		//左上角
		else if (x_player < x_enemy && y_player > y_enemy)
			_angle= 90-angle;
		
		// //右上角
		else if (x_player > x_enemy && y_player > y_enemy)
		    _angle= 270+angle;
		if(speed_shoot == 100){
		fire(0);
		speed_shoot = 0;
		}	
	

	}


		
	};
	// 守卫以z字行进行移动 巡逻。
	bool move (double dx, double dy) { 

	int old_x = _x;
	int old_y = _y;

	
	
	if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
							 (int)((_y + dy) / Environnement::scale))    || ( (int)old_x/Environnement::scale ==(int)((_x+dx) / Environnement::scale)  && (int)old_y/Environnement::scale ==(int)((_y+dy) / Environnement::scale) ) )
	{
		
		if(_l->data((int)(old_x / Environnement::scale) , (int)(old_y / Environnement::scale) ) == 1)
			((Labyrinthe*)_l)->setData((int)(old_x / Environnement::scale),(int)(old_y / Environnement::scale),EMPTY);
		
		_x += dx;
		_y += dy;

		((Labyrinthe*)_l)->setData((int)(_x / Environnement::scale),(int)(_y / Environnement::scale),1);
	
		return true;
	}
	
	return false;
 }

	//守卫射击
	void fire (int angle_vertical) {
	
		int angle_H = 360-_angle+random()%5;
		if (angle_H <0 )
		{
			/* code */
			angle_H = 0;
		}
		
		_fb -> init (/* position initiale de la boule */ _x, _y, 10.,
				 /* angles de vis�e */ angle_vertical, angle_H);
		
		
	}
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy) { 



	if(
		(int)((_fb -> get_x () + dx) / Environnement::scale)  == (int)(_l->_guards [0] -> _x / Environnement::scale)
		&&
		(int)((_fb -> get_y () + dy) / Environnement::scale) ==(int)(_l->_guards [0] -> _y / Environnement::scale)
	){
		
		((Chasseur*)_l->_guards[0])->hurt();
		return false;
	}
	else if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
							 (int)((_fb -> get_y () + dy) / Environnement::scale)))
	{
		
		return true;
	}
	else 
		return false;

	}

//判断玩家和守卫之间是否有墙
bool existWall(){  
	
	 int x_player =  (int)(_l->_guards[0] ->_x/Environnement::scale);
	 int y_player = (int)(_l->_guards[0] -> _y/Environnement::scale);
	 int x_gardien = (int)(this->_x/Environnement::scale);
	 int y_gardien = (int)(this->_y/Environnement::scale);




		for( int i = min(x_player,x_gardien); i<= max(x_player, x_gardien) ;i++ )
			for (int j = min(y_player,y_gardien); j <= max(y_player,y_gardien); j++)
			{
				/* code */

				if((i== x_player && j == y_player) || (i==x_gardien && j == y_gardien))
				continue;
				
				if(_l->data( i, j) == 1){
					
					return true;
				}
					

			}
		return false;

}

 };

#endif
