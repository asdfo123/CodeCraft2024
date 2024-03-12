#include <bits/stdc++.h>
using namespace std;

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;

struct Robot
{
    int x, y, goods;
    int status; // 0: recovering, 1: moving
    int mbx, mby;
    int logtime, logx, logy;
    int item_value;
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
}robot[robot_num + 10];

struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int now_items;
    int now_items_value;
    int boat_is_coming;
    int id;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
    bool operator < (const Berth &b) const {
        return transport_time < b.transport_time;
    }
}berth[berth_num + 10],berth1[berth_num + 10];

struct Boat
{
    int num, pos, status;
    int startzhen;
}boat[10];

struct Item{
    int x, y;
    int startzhen;
    int val;
    Item(int x, int y, int startzhen, int val) : x(x), y(y), startzhen(startzhen), val(val) {}
    bool operator==(const Item& other) const {
        return x == other.x && y == other.y;
    }
};
int money, boat_capacity, id;
char ch[N][N];
vector<Item> items;
int berth_optimal_sequence[10];
void Init()
{
    for(int i = 0; i < n; i ++)
        scanf("%s", ch[i]);
    for(int i = 0; i < berth_num; i++)
    {
        int idd;
        scanf("%d", &idd);
        scanf("%d%d%d%d", &berth[idd].x, &berth[idd].y, &berth[idd].transport_time, &berth[idd].loading_speed);
        berth[i].now_items = 0;
        berth[i].boat_is_coming = 0;
//        berth[idd].x++;
//        berth[idd].y++;//更平均
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input(int startzhen)
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for(int i = 1; i <= num; i++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        items.push_back(Item{x, y, startzhen, val});
//        sum_item += val;
//        num_item++;
    }
    for(int i = 0; i < robot_num; i ++)
    {
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
        robot[i].mbx = robot[i].x;
        robot[i].mby = robot[i].y;
    }
    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[100];
    scanf("%s", okk);
    return id;
}

const int INF = INT_MAX - 10;

// 0: right, 1: left, 2: up, 3: down
const int dx[] = {0, 0, -1, 1};
const int dy[] = {1, -1, 0, 0};
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};
int dist[N][N];
//BFS: return a 2D vector of shortest distance from start to each point.
void bfs(const Point& start,int mode=0) {
    for(int i = 0;i < N;i++)
        for(int j = 0;j < N;j++)
            dist[i][j] = INF;
    queue<Point> q;
    q.push(start);
    dist[start.x][start.y] = 0;

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && ch[nx][ny] != '#' && ch[nx][ny] != '*' && dist[nx][ny] == INF) {
//                if(mode == 0&&(nx<start.x-30||nx>start.x+30||ny<start.y-30||ny>start.y+30)) continue;
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                q.push(Point(nx, ny));
            }
        }
    }
}

priority_queue<pair<int,int>> q;
int findNextMove(int robot_id, bool goods,int nowzhen) {
    bfs(Point(robot[robot_id].x, robot[robot_id].y),goods);
    if(robot[robot_id].logtime + 8 < nowzhen) //8好
    {
        if(robot[robot_id].logx == robot[robot_id].x && robot[robot_id].logy == robot[robot_id].y){
            int pianyi = rand()%4;
            for(int dir = 0;dir < 4;dir++)
            {
                int dirr = (dir + pianyi) % 4;
                int nx = robot[robot_id].x + dx[dirr];
                int ny = robot[robot_id].y + dy[dirr];
                if(nx >= 0 && nx < n && ny >= 0 && ny < n && ch[nx][ny] != '#' && ch[nx][ny] != '*')
                {
                    int flag = 1;
                    for(int i = 0;i < robot_num;i++)
                    {
                        if(i == robot_id) continue;
                        if(robot[i].mbx == nx && robot[i].mby == ny)
                        {
                            flag = 0;
                            break;
                        }
                    }
                    if(flag == 1) {
                        robot[robot_id].mbx = nx;
                        robot[robot_id].mby = ny;
                        return dirr;
                    }
                }
            }
        }
        robot[robot_id].logtime = nowzhen;
        robot[robot_id].logx = robot[robot_id].x;
        robot[robot_id].logy = robot[robot_id].y;
    }
    long long min_distance = INF;
    Point targetPos(0, 0);
    long long status_metrics = (long long)INF*2;
    Item targetItem(0, 0, 0, 0);
    if(goods == 0)//find goods
    {
        for (auto item : items) {
            long long distance = dist[item.x][item.y];
            if (2*distance - (long long)item.val < status_metrics){
                status_metrics = 2*distance - (long long)item.val;
                min_distance = distance;
                targetPos = Point(item.x, item.y);
                targetItem = Item(item.x, item.y, 0, item.val);
            }
        }
        if(status_metrics >= INF) return -1;
    }
    else //find berth
    {
        for (int i = 0; i < berth_num; i++) {
            int distance = dist[berth[i].x][berth[i].y];
//            cerr<<berth[i].x<<" "<<berth[i].y<<endl;
//            cerr<<distance<<endl;
            if (distance < min_distance) {
                min_distance = distance;
                targetPos = Point(berth[i].x, berth[i].y);
            }
        }
        if(min_distance == INF) return -1;
    }
    if(min_distance == 0)
    {
        if(goods == 0) robot[robot_id].item_value = targetItem.val;
        return 4;
    }
    //cerr<<"1"<<endl;
    int x = targetPos.x;
    int y = targetPos.y;
    int dir1 = 0;
    //cerr<<dist[x][y]<<endl;
    while (dist[x][y] > 1) {
        for (int dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && dist[nx][ny] == dist[x][y] - 1) {
                x = nx;
                y = ny;
                break;
            }
        }
    }
    for(int i = 0;i < robot_num;i++)
    {
        if(i == robot_id) continue;
        if(robot[i].mbx == x && robot[i].mby == y)
        {
            return -1;
        }
    }
    //cerr<<"2"<<endl;
    for(int dir = 0; dir < 4; dir++){
        int nx = x - dx[dir];
        int ny = y - dy[dir];
        if(nx == robot[robot_id].x && ny == robot[robot_id].y){
            dir1 = dir;
            break;
        }
    }
    //cerr<<"3"<<endl;
    robot[robot_id].mbx = x;
    robot[robot_id].mby = y;
//    if(min_distance == 1)
//    {
//        return 5+dir1;
//    }
    return dir1;// 0: right, 1: left, 2: up, 3: down
}
void handle_item(int nowzhen)
{
    for(Item &i : items)
    {
        if(i.startzhen + 1000 < nowzhen)
        {
            items.erase(remove(items.begin(), items.end(), i), items.end());
//            sum_item -= i.val;
//            num_item--;
        }

    }
}
void handle_robot(int robot_id,int nowzhen)
{
    if(robot[robot_id].status == 0) //is_recovering
        return;
    if(robot[robot_id].goods == 0)
    {
        int dir = findNextMove(robot_id, robot[robot_id].goods,nowzhen);
        if(dir == 4)
        {
            printf("get %d\n", robot_id);
            items.erase(remove(items.begin(), items.end(), Item{robot[robot_id].x, robot[robot_id].y, 0, 0}), items.end());

        }
        else if(dir >= 0 && dir <= 3) printf("move %d %d\n", robot_id, dir);
//        else if(dir >= 5)
//        {
//            printf("move %d %d\n", robot_id, dir-5);
//            printf("get %d\n", robot_id);
//            items.erase(remove(items.begin(), items.end(), Item{robot[robot_id].mbx, robot[robot_id].mby, 0, 0}), items.end());
//        }
        else return;
    }
    else
    {
        int dir = findNextMove(robot_id, robot[robot_id].goods, nowzhen);
        if(dir == 4)
        {
            printf("pull %d\n", robot_id);
            for(int i = 0;i < berth_num;i++)
            {
                if(berth[i].x == robot[robot_id].x && berth[i].y == robot[robot_id].y)
                {
                    berth[i].now_items += 1;
                    berth[i].now_items_value += robot[robot_id].item_value;
                    q.push(make_pair(berth[i].now_items,i));
//                    q.push(make_pair(berth[i].now_items_value,i));
                    break;
                }
            }
        }
        else if(dir >= 0 && dir <= 3) printf("move %d %d\n", robot_id, dir);
//        else if(dir >= 5)
//        {
//            printf("move %d %d\n", robot_id, dir-5);
//            printf("pull %d\n", robot_id);
//            for(int i = 0;i < berth_num;i++)
//            {
//                if(berth[i].x == robot[robot_id].mbx && berth[i].y == robot[robot_id].mby)
//                {
//                    berth[i].now_items += 1;
//                    q.push(make_pair(berth[i].now_items,i));
//                    break;
//                }
//            }
//        }
        else return;
    }

}
void handle_boat(int boat_id,int nowzhen)
{
    if(boat[boat_id].status == 0) return;
    if(boat[boat_id].status == 1 && boat[boat_id].pos == -1){
        if(!q.empty())
        {
            int i = q.top().second;
            if(berth[i].boat_is_coming == 0)
            {
                printf("ship %d %d\n", boat_id, i);
                berth[i].boat_is_coming = 1;
            }
            q.pop();

        }
//        for(int i = 0;i < berth_num;i++)
//        {
//            if(berth[i].now_items > 0 && berth[i].boat_is_coming == 0)
//            {
//                printf("ship %d %d\n", boat_id, i);
//                berth[i].boat_is_coming = 1;
//                break;
//            }
//        }
    }
    if(boat[boat_id].status == 1 && boat[boat_id].pos != -1){
        if(boat[boat_id].startzhen == 0) boat[boat_id].startzhen = nowzhen;
        if((nowzhen-boat[boat_id].startzhen)*berth[boat[boat_id].pos].loading_speed >= berth[boat[boat_id].pos].now_items) //100
        {
            printf("ship %d\n",boat_id);
            berth[boat[boat_id].pos].now_items = 0;
            berth[boat[boat_id].pos].boat_is_coming = 0;
            berth[boat[boat_id].pos].now_items_value = 0;
            boat[boat_id].startzhen = 0;
        }
        else if((nowzhen-boat[boat_id].startzhen)*berth[boat[boat_id].pos].loading_speed >= boat_capacity) //100
        {
            printf("ship %d\n",boat_id);
            berth[boat[boat_id].pos].now_items -= (nowzhen-boat[boat_id].startzhen)*berth[boat[boat_id].pos].loading_speed;
            berth[boat[boat_id].pos].now_items_value = 0;
            berth[boat[boat_id].pos].boat_is_coming = 0;
            boat[boat_id].startzhen = 0;
        }
        else if(nowzhen + berth[boat[boat_id].pos].transport_time > 14998)
        {
            printf("ship %d\n",boat_id);
            berth[boat[boat_id].pos].now_items = 0;
            berth[boat[boat_id].pos].now_items_value = 0;
            berth[boat[boat_id].pos].boat_is_coming = 0;
            boat[boat_id].startzhen = 0;
        }
    }
}
int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen++)
    {
        int id = Input(zhen);
        handle_item(id);
        for(int i = 0; i < robot_num; i++){
            //cerr<<zhen<<" "<<i<<endl;
            handle_robot(i,id);
        }
        for(int i = 0;i < 5;i++)
        {
            handle_boat(i,id);
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}

