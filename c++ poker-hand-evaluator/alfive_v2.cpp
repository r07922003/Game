#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <limits.h>

using namespace std;

const int mask_of_poker_num = 0x00f;
const int max_number_of_poker = 13;
const int number_of_hand_cards = 5;

bool is_same_color(unordered_set<int>& colors){
    return colors.size()==1;
}
bool is_continue(unordered_map<int,int>& count_of_numbers,int wang,bool& all_bigger_than_nine){
    int size = number_of_hand_cards-wang;
    if(count_of_numbers.size()!=size) return false;
    else if(all_bigger_than_nine) return all_bigger_than_nine;
    int MIN = INT_MAX;
    int MAX = INT_MIN;
    for(auto& [num,freq]:count_of_numbers){
        MIN = min(MIN,num);
        MAX = max(MAX,num);
    }
    return MAX-MIN >= 4-wang;
}
int calculate_with_laizi(const vector<int>& input){
    int wang = 0;
    unordered_map<int,int> count_of_numbers;
    unordered_set<int> distinct_of_colors;
    bool all_bigger_than_nine = true;
    for(auto& num:input){
        if(num!=0x50f && num!=0x610){
            distinct_of_colors.insert(num>>8);
            int number = num & mask_of_poker_num;
            if(number<=9) all_bigger_than_nine = false;
            int cur_poker_num = ((number)-1) % max_number_of_poker;
            count_of_numbers[cur_poker_num]++;   
        }
        else wang++;
    }
    //start to calculate
    bool same_colors = is_same_color(distinct_of_colors);
    bool number_continue = is_continue(count_of_numbers,wang,all_bigger_than_nine);
    //皇家同花顺
    if(same_colors && number_continue && all_bigger_than_nine) return 1;
    //同花顺
    if(same_colors && number_continue) return 2;
    if(wang){
        if(count_of_numbers.size()<=2){
            int bigger_than_two = 0;
            for(auto& [num,freq]:count_of_numbers){
                if(freq>=2) bigger_than_two++;
            }
            //金刚
            if(bigger_than_two<2) return 3;
            //葫芦
            else return 4;
        }
        //同花
        if(same_colors) return 5;
        //顺子
        if(number_continue) return 6;
        //可換兩張排的話一定可以三条
        if(wang==2) return 7;
        else{
            int MAX_freq = 0;
            for(auto& [num,freq]:count_of_numbers) MAX_freq=max(MAX_freq,freq);
            if(MAX_freq>=2) return 7;
            else return 9;
        }
    }
    else{
        //金刚
        if(count_of_numbers.size()==2 && count_of_numbers.begin()->second==1 || count_of_numbers.begin()->second==4-wang) return 3;
        //葫芦
        if(count_of_numbers.size()==2 && count_of_numbers.begin()->second==2 || count_of_numbers.begin()->second==3) return 4;
        //同花
        if(same_colors) return 5;
        //顺子
        if(number_continue) return 6;
        if(count_of_numbers.size()==3){
            bool has_three = false;
            for(auto& [num,count]:count_of_numbers){
                if(count==3) has_three = true;
            }
            //三条
            if(has_three) return 7;
            //两对
            else return 8;
        }
        //一对
        if(count_of_numbers.size()==4) return 9;
        //高牌
        return 10;
    }
    //will not reach here
    return -1;
}