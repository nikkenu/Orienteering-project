/*
####################################################################
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project3: Orienteering                                           #
# Program description: Program has different routes and route's    #
# have different path and path's height varies.                    #
# Basicly we examine paths                                         #
# File: routes.cpp                                                 #
# Description: Starts executing the program.                       #
#                                                                  #
# Author: Niklas Nurminen, 283756., niklas.nurminen@tuni.fi        #
####################################################################
*/

#include "routes.hh"
#include <cmath>
#include <algorithm>

// Constructor and deconstructor
Routes::Routes(){

}
Routes::~Routes(){
    route_.clear();
    points_.clear();
}

//Set map max dimensions
void Routes::set_map_size(int width, int height){
    map_width_ = width;
    map_height_ = height;
}

//Make new point, same point's details to it then insert it to map
void Routes::add_point(std::string& name, int x, int y, int height, char marker){

    Point* point = new Point({name, x, y, height, marker});
    points_.insert({name, point});

}

//Push route names and route's path to map datastructure
//Map key: Route's name example. Shorter, Longer
//Inside map is vector datastructure which includes the route's path
bool Routes::connect_route(std::string& from, std::string& to, std::string& route_name){
    if(points_.find(from) == points_.end() or points_.find(to) == points_.end()){
        return false;
    }else{
        if(route_.find(route_name) == route_.end()){
            route_.insert({route_name,{from, to}});
        }else{
            route_.at(route_name).push_back(to);
        }
        return true;
    }
}

//Print_map which as x-axis and y-axis, looks like matrix
void Routes::print_map() const{

    //Prints x-axis numbers with specific spacing
    std::cout << " ";
    for(int x = 1; x <= map_width_; ++x){
        std::cout << std::setw(3) << x;
    }
    std::cout << std::endl;

    //Print map, if empty "." otherwise it has marker
    for(int y = 1; y <= map_height_; ++y){
        std::cout << std::setw(2) << y << "  ";
        for(int x = 1; x <= map_width_; ++x){
            bool find_point = false;

            //Check if some point has same x and y values, if yes: replace the dot with sign
            for(auto const& point : points_){
                if(x == point.second->x_coordinate and y == point.second->y_coordinate){
                    find_point = true;
                    std::cout << point.second->marker;
                }
            }
            if(find_point == false) std::cout << ".";

            if(x < map_width_) std::cout << "  ";

            if(x == map_width_) std::cout << "\n";
        }
    }

}


//For-loop goes through all the routes and also prints route's spots.
void Routes::print_route_names() const{
    std::cout << "Routes:" << std::endl;

    for(const auto& route_length : route_){
        std::cout << " - " << route_length.first << std::endl;
    }
}

//For-loop goes through all points, and prints point's name and marker
void Routes::print_points() const{
    std::cout << "Points:" << std::endl;

    for(const auto& point : points_){
        std::cout << " - " << point.first << " : " << point.second->marker <<std::endl;
    }
}

//First check if route even exists, if exists for-loop prints route's points
void Routes::print_route(const std::string& name) const{
    if(route_.find(name) == route_.end()){
        std::cout << "Error: Route named " << name <<  " can't be found" << std::endl;
    }else{
        std::cout << route_.at(name).at(0) << std::endl;
        for(unsigned int i = 1; i < route_.at(name).size(); ++i){
            std::cout << " -> " << route_.at(name).at(i) << std::endl;
        }
    }
}

//First check if route exists, if exists calculate total lenght of between route's points
void Routes::route_length(const std::string& name) const{
   double total_lenght = 0;

   //If find the road name, use for-loop to calculate x and y coordinates between each other
   if(route_.find(name) != route_.end()){

       //For-loop starts at index one, because it otherwise it would crash in the end
       for(unsigned int i = 1; i < route_.at(name).size(); ++i){
           int from_x_coord = points_.at(route_.at(name).at(i-1))->x_coordinate;
           int from_y_coord = points_.at(route_.at(name).at(i-1))->y_coordinate;
           int to_x_coord = points_.at(route_.at(name).at(i))->x_coordinate;
           int to_y_coord = points_.at(route_.at(name).at(i))->y_coordinate;

           //Calculation was done with pythagoras (a²=b²+c²)
           total_lenght += sqrt(pow(to_x_coord-from_x_coord,2)+pow(to_y_coord-from_y_coord,2));
       }

       std::cout << "Route "<< name << " length was " << std::setprecision(2) << total_lenght << std::endl;
   }else{
       std::cout << "Error: Route named " << name << " can't be found" << std::endl;
   }

}

/*
  greatest_rise finds biggest continuous rise from every route,
  which contains the parameter.
*/
void Routes::greatest_rise(const std::string& point_name) const{

    //These variables tells us does the point even exists on route and does it have rise.
    bool point_exists = false;
    int max_rise = 0;

    //push route's name if it has biggest rise or same rise
    std::vector<std::string> route_names;

    //Goes through all possible routes example.. shorter, longer
    for(const auto& route : route_){

        //These variables are for single route, so they reset automatically when routename changes
        bool last_one = false;
        bool point_on_route = false;
        int point_height = 0;
        int route_rise = 0;

        //If find parameter from route's path, can start exploring the path more.
        //Also can say that point_exists and set the height of the point to variable.
        if(std::find(route.second.begin(), route.second.end(), point_name) != route.second.end()){
            point_height = points_.at(point_name)->height;
            point_exists = true;

            //First need to find parameter's point location on route, because
            //need to explore heights after it
            for(unsigned int i = 0; i < route.second.size(); ++i){
                if(point_on_route == false and point_name == route.second.at(i)){
                    point_on_route = true;
                    if(i == route.second.size()-1){
                        last_one = true;
                        break;
                    }
                }

                //If found the parameter's point and it's the last point on path:
                //Path doesnt rise or lower, so break the loop.
                if(last_one == true and point_on_route == true){
                    break;

                //With all those conditions, we can compare if next height is bigger or lower
                //If bigger we calculate the rise and add it, or if lower we can say it's the last one
                //and break the loop
                }else if(point_on_route == true and point_name != route.second.at(i)){
                    if(points_.at(route.second.at(i))->height > point_height){
                        route_rise += points_.at(route.second.at(i))->height - point_height;
                        point_height = points_.at(route.second.at(i))->height;
                    }else{
                        last_one = true;
                        break;
                    }
                }
            }

            //Rise can be bigger or same. If bigger: clean the vector and add route's name to it
            //If same: add route's name to it, even tho it's not empty
            if(route_rise > max_rise){
                max_rise = route_rise;
                route_names.clear();
                route_names.push_back(route.first);
            }else if(route_rise == max_rise and max_rise > 0){
                route_names.push_back(route.first);
            }

        }
            
    }

    //Simple output with quite clear conditions. Three possible outcomes
    //1: Point doesn't exists on the route. 2: Next point doesnt rise. 3: Greatest rise
    if(max_rise == 0 and point_exists == true){
        std::cout << "No route rises after point " << point_name << std::endl;
    }else if(point_exists == false){
        std::cout << "Error: Point named " << point_name << " can't be found" << std::endl;
    }else{
        std::cout << "Greatest rise after point " << point_name << ", " << max_rise <<" meters, is on route(s):" << std::endl;
        for(const auto& i : route_names){
            std::cout << " - " << i << std::endl;
        }
    }

}
