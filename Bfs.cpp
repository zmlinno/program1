//
//  Bfs.cpp
//  CPP
//
//  Created by 张木林 on 11/19/24.
//




#include <stdio.h>


#include<iostream>
#include<vector>
#include<queue>
using namespace std;

//BFS函数
void bfs(int startNode,vector<vector<int>>& graph)
{
    int n = graph.size();//节点总数
    vector<bool>visited(n,false);//标记节点是否访问过
    queue<int>q;//队列
    
    //初始化BFS
    q.push(startNode);
    visited[startNode] = true;
    
    cout<<"BFS traversal order: ";
    
    while(!q.empty())
    {
        int current = q.front();//队列头部节点
        q.pop();
        
        //输出当前节点
        cout<<current<<" ";
        
        //遍历当前节点的邻居
        for(int neighbor:graph[current])
        {
            if(!visited[neighbor])
            {
                q.push(neighbor);//将未访问的邻居加入队列
                visited[neighbor] = true;//标记为已访问
                
            }
        }
    }
    cout<<endl;
}
int main()
{
    //定义一个圈 - 领接列表
    vector<vector<int>> grpah = {
        {1,2},
        {0,3,4},
        {0,4},
        {1,5},
        {1,2,5},
        {3,4}
        
    };
    bfs(0,grpah);
    return 0;
}
