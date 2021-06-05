/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision.cpp
Project: GAM150
Author:
-----------------------------------------------------------------*/
#include "Map.h"
#include <fstream> // to load file
#include<iostream>
#include <algorithm>

using namespace doodle;

void Map::Set_up(int level)
{
    map.clear();
    std::ifstream readFile;

    readFile.open("assets/Map" + to_string(level - (static_cast<int>(State::LEVEL_1) - 1)) + ".txt");
    int width = 0;
    int height = 0;

    if (readFile.is_open())
    {
        while (!readFile.eof())
        {
            char a;
            readFile >> a;
            switch (a)
            {
            case '0': map.push_back(info{ math::ivec2{width,height},Type::ROAD });
                break;
            case '1': map.push_back(info{ math::ivec2{width,height},Type::WALL });
                break;
            case '2':map.push_back(info{ math::ivec2{width,height},Type::RADAR });
                break;
            case '3':map.push_back(info{ math::ivec2{width,height},Type::TREASURE });
                break;
            }
            if (width < map_width - 1)
            {
                width++;
            }
            else if (width == (map_width - 1))
            {
                width = 0;
                height++;
            }
        }
    }
    readFile.close();

    for (auto& i : map)
    {
        int rand = doodle::random(0, 15);
        i.random_num = rand;
    }
    bomb_target_time = 2;
}

doodle::Image& Map::Set_wall(info& value)
{
    int wall_count = 0;
    int index = 0;
    for (int i = 0; i < map.size(); i++)
    {
        if (map[i].position == value.position)
        {
            index = i;
            break;
        }
    }

    if (index < map_width)
    {
        return WallWall;
    }
    else if (index + 1 % (map_width) == 0)
    {
        return WallWall;
    }
    else if (index % (map_width) == 0)
    {
        return WallWall;
    }
    else if (index > map.size() - (map_width + 1))                       
    {
        return WallWall;
    }
    else
    {
        if (map[index - map_width - 1].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index - map_width].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index - map_width + 1].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index - 1].type == Type::WALL)
        {
            wall_count++;
        }
        
        if (map[index + 1].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index + map_width - 1].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index + map_width].type == Type::WALL)
        {
            wall_count++;
        }
        if (map[index + map_width + 1].type == Type::WALL)
        {
            wall_count++;
        }
    }
    switch (wall_count) {
    case 1:
        break;
    case 2:
    case 3:
        if (map[index - map_width - 1].type == Type::WALL)
        {
            return Wall_Edge_2;
        }
        else if (map[index - map_width + 1].type == Type::WALL)
        {
            return Wall_Edge_3;
        }
        else if (map[index + map_width - 1].type == Type::WALL)
        {
            return Wall_Edge_1;
        }
        else if ( map[index + map_width + 1].type == Type::WALL)
        {
            return Wall_Edge_4;
        }
        else
            break;
    case 4:
    case 5:
    case 6:
        if (map[index - map_width].type != Type::WALL)
        {
            return Wall_Side_Up;
        }
        else if ( map[index - 1].type != Type::WALL)
        {
            return Wall_Side_Left;
        }
        else if ( map[index + 1].type != Type::WALL)
        {
            return Wall_Side_Right;
        }
        else if ( map[index + map_width].type != Type::WALL)
        {
            return Wall_Side_Down;
        }
        else
            break;
    case 7:
        if (map[index - map_width - 1].type != Type::WALL)
        {
            return Wall_Corner_4;
        }
        else if ( map[index - map_width + 1].type != Type::WALL)
        {
            return Wall_Corner_1;
        }
        else if ( map[index + map_width - 1].type != Type::WALL)
        {
            return Wall_Corner_3;
        }
        else if ( map[index + map_width + 1].type != Type::WALL)
        {
            return Wall_Corner_2;
        }
        else
            break;
    case 8:
        break;
    }
    
    return WallWall;
}



void Map::Draw(Camera& camera)
{

    for (auto& i : map)
    {

        switch (i.type)
        {
        case Type::WALL:
            push_settings();
            set_fill_color(0, 0, 0);
            set_outline_width(1);
            set_outline_color(255);
            draw_image(Set_wall(i), (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
            pop_settings();
            break;
        case Type::ROAD:
            push_settings();
            set_fill_color(0, 0, 255);
            set_outline_width(1);
            set_outline_color(255);
            switch (i.random_num)
            {
            case 0: {draw_image(Road1, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
                break; }
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                draw_image(Road2, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
                break;
            }
            case 13:
            case 14:
            {
                draw_image(Road3, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
                break;
            }
            }
            pop_settings();
            break;
        case Type::RADAR:
            push_settings();
            draw_image(Road1, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
            draw_image(Radar, (i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size, block_size);
            pop_settings();
            break;
        case Type::TREASURE:
            push_settings();
            set_fill_color(255, 255, 0);
            set_outline_width(1);
            set_outline_color(255);
            draw_rectangle((i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size);
            pop_settings();
            break;
        case Type::EXIT:
            push_settings();
            set_fill_color(255, 40, 0);
            set_outline_width(1);
            set_outline_color(255);
            draw_image(Breakable_wall, (i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size, block_size);            pop_settings();
            break;
        case Type::DOG_CHEW:
            push_settings();
            draw_image(Road2, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
            draw_image(Dog_chew, (i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size, block_size);
            pop_settings();
            break;
        case Type::BOMB:
            push_settings();
            set_fill_color(200, 200, 0);
            set_outline_width(1);
            set_outline_color(255);
            draw_image(Road2, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
            draw_image(Bomb, (i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size, block_size);
            pop_settings();
            break;
        case Type::CAN_ESCAPE:
            push_settings();          
            bomb_target_time -= doodle::DeltaTime;
            draw_image(Explode.image, (i.position.x + camera.Get_position().x) * block_size, (i.position.y + camera.Get_position().y) * block_size, block_size,block_size,Explode.GetDrawPos().x,0);
            Explode.Update();
            if (bomb_target_time < 0)
            {
                Explode.currAnim = 0;
                draw_image(Escape, (i.position.x + camera.Get_position().x) * block_size - 25, (i.position.y + camera.Get_position().y) * block_size - 25, block_size * 2.5, block_size * 2.5);
            }
            pop_settings();
            break;
        }

    }

}