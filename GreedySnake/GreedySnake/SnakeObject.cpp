#include "SnakeObject.h"
#include <iostream>

SnakeObject::SnakeObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f),
		velocity),frameCounter(1),nextdir(this->Velocity)
{
	snake.push_back(Body(pos.x, pos.y));
	snake.front().velocity = this->Velocity;
	AddBody(pos);
	AddBody(pos);
}

SnakeObject::~SnakeObject()
{
}

//添加蛇身
void SnakeObject::AddBody(glm::vec2 pos) {
	pos = snake.front().pos + this->Velocity;
	snake.push_front(Body(pos.x, pos.y));
	this->Position = pos;
	snake.front().velocity = this->Velocity;
}

//移动蛇身，玩家模式
void SnakeObject::Move(GLfloat dt, Algorithm &algorithm) {
	//因为要移动蛇身，之前棋盘的记录全部要重置
	algorithm.Reset();
	//移动身体
	++frameCounter;
	//移动每段蛇身
	for (std::list<Body>::iterator it = snake.begin(); it != snake.end(); ++it) {
		it->pos += it->velocity*perFrame;
		algorithm.Place(it->pos.x / this->Size.x, it->pos.y / this->Size.y);
	}
	this->Position = snake.front().pos;
	if (frameCounter == totalFrame) {
		frameCounter = 1;
		//更新每段蛇身的速度方向，前一段蛇身的前一次速度赋给下一段蛇身的当前速度
		glm::vec2 prevVel = this->Velocity;
		std::list<Body>::iterator it = snake.begin();
		it->velocity = nextdir;
		this->Velocity = nextdir;//记录蛇头的速度
		for (++it; it != snake.end(); ++it) {
			glm::vec2 tmp = it->velocity;
			it->velocity = prevVel;
			prevVel = tmp;
		}
	}
}

void SnakeObject::MoveByAi(glm::vec2 step, Algorithm &algorithm) {
	algorithm.Reset();
	//std::cout << step.x << "," << step.y << std::endl;
	//std::cout << snake.size() << std::endl;
	//std::cout << snake.front().pos.x << "," << snake.front().pos.y << std::endl;
	//std::cout << "begin\n";
	for (std::list<Body>::iterator it = snake.begin(); it != snake.end(); ++it) {
		glm::vec2 prev = it->pos;
	//	std::cout << "exe??" << std::endl;
		//std::cout << it->pos.x << "," << it->pos.y << "-";
		it->pos = step;
		step = prev;
		algorithm.Place(it->pos.x / this->Size.x, it->pos.y / this->Size.y);
	}
	//std::cout << "end\n";
	//std::cout << std::endl;
	this->Position = snake.front().pos;
}

void SnakeObject::Draw(SpriteRenderer &renderer) {
	for (auto & body : snake) {
		renderer.DrawSprite(this->Sprite, body.pos, this->Size, this->Rotation, body.Color);
	}
}

//重置
void SnakeObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	snake.clear();
	this->Position = position;
	this->Velocity = velocity;
	this->nextdir = velocity;
	snake.push_back(Body(position.x, position.y));
	snake.front().velocity = this->Velocity;
	AddBody(position);
	AddBody(position);
	frameCounter = 1;
}

glm::vec2 SnakeObject::GetHeadPos() {//获取蛇头位置
	return snake.front().pos;
}

bool SnakeObject::isCollisionSelf(){//是否吃到自己
	std::list<Body>::iterator it = snake.begin();
	for (++it; it != snake.end(); ++it) {
		if (CollisionDetect::CheckCollision2(snake.front().pos, this->Size, it->pos, this->Size))
		{
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}