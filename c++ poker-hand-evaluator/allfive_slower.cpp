#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>
using namespace std;

/*
算牌型
*/
const int mask_of_poker_num = 0x00f;
const int max_number_of_poker = 13;
const int number_of_hand_cards = 5;

bool is_same_color(unordered_set<int>& colors){
    return colors.size()==1;
}
bool is_continue(map<int,int>& count_of_numbers){
    if(count_of_numbers.size()!=number_of_hand_cards) return false;
    vector<pair<int,int>> temp_array = vector<pair<int,int>>(count_of_numbers.begin(),count_of_numbers.end());
    int next = -1;
    int count_of_continue = 1;
    for(int i=0;i<number_of_hand_cards*2;i++){
        int cur_index = i % number_of_hand_cards;
        if(temp_array[cur_index].first!=next){
            count_of_continue = 1;
        }
        else count_of_continue++;
        next = (temp_array[cur_index].first + 1) % max_number_of_poker;
        if(count_of_continue==5) return true;
    }
    return false;
}
int calculate(const vector<int>& input){
    //vector<poker *> candidate;
    map<int,int> count_of_numbers;
    unordered_set<int> distinct_of_colors;
    bool all_bigger_than_nine = true;
    for(auto& num:input){
        /*
        poker *cur = new poker(num);
        candidate.push_back(cur);
        */
        distinct_of_colors.insert(num>>8);
        int number = num & mask_of_poker_num;
        if(number<=9) all_bigger_than_nine = false;
        int cur_poker_num = ((number)-1) % max_number_of_poker;
        count_of_numbers[cur_poker_num]++;
    }
    //start to calculate
    bool same_colors = is_same_color(distinct_of_colors);
    bool number_continue = is_continue(count_of_numbers);
    //皇家同花顺
    if(same_colors && number_continue && all_bigger_than_nine) return 1;
    //同花顺
    if(same_colors && number_continue) return 2;
    //金刚
    if(count_of_numbers.size()==2 && count_of_numbers.begin()->second==1 || count_of_numbers.begin()->second==4) return 3;
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