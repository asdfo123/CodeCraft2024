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
    int boat_is_coing;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
}berth[berth_num + 10];

struct Boat
{
    int num, pos, status;
    int startzhen;
}boat[10];

struct Item{
    int x, y;
    int startzhen;
    int val;
    bool operator==(const Item& other) const {
        return x == other.x && y == other.y;
    }
};
int money, boat_capacity, id;
char ch[N][N];
vector<Item> items;

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
        berth[i].boat_is_coing = 0;
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
                if(mode == 0&&(nx<start.x-30||nx>start.x+30||ny<start.y-30||ny>start.y+30)) continue;
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                q.push(Point(nx, ny));
            }
        }
    }
}


int findNextMove(int robot_id, bool goods,int nowzhen) {
    bfs(Point(robot[robot_id].x, robot[robot_id].y),goods);
    if(robot[robot_id].logtime + 10 < nowzhen)
    {
        if(robot[robot_id].logx == robot[robot_id].x && robot[robot_id].logy == robot[robot_id].y){
            for(int dir = 0;dir < 4;dir++)
            {
                int nx = robot[robot_id].x + dx[dir];
                int ny = robot[robot_id].y + dy[dir];
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
                        return dir;
                    }
                }
            }
        }
        robot[robot_id].logtime = nowzhen;
        robot[robot_id].logx = robot[robot_id].x;
        robot[robot_id].logy = robot[robot_id].y;
    }
    int min_distance = INF;
    Point targetPos(0, 0);
    if(goods == 0)//find goods
    {
        for (auto item : items) {
            int distance = dist[item.x][item.y];
            if (distance < min_distance) {
                min_distance = distance;
                targetPos = Point(item.x, item.y);
            }
        }
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
    }
    if(min_distance == INF) return -1;
    if(min_distance == 0) return 4;
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
    return dir1;// 0: right, 1: left, 2: up, 3: down
}
void handle_item(int nowzhen)
{
    for(Item &i : items)
    {
        if(i.startzhen + 1000 < nowzhen)
            items.erase(remove(items.begin(), items.end(), i), items.end());
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
        else return;
    }
    else
    {
        int dir = findNextMove(robot_id, robot[robot_id].goods,nowzhen);
        if(dir == 4)
        {
            printf("pull %d\n", robot_id);
            for(int i = 0;i < berth_num;i++)
            {
                if(berth[i].x == robot[robot_id].x && berth[i].y == robot[robot_id].y)
                {
                    berth[i].now_items += 1;
                    break;
                }
            }
        }
        else if(dir >= 0 && dir <= 3) printf("move %d %d\n", robot_id, dir);
        else return;
    }

}
void handle_boat(int boat_id,int nowzhen)
{
    if(boat[boat_id].status == 0) return;
    if(boat[boat_id].status == 1 && boat[boat_id].pos == -1){
        for(int i = 0;i < berth_num;i++)
        {
            if(berth[i].now_items > 0 && berth[i].boat_is_coing == 0)
            {
                printf("ship %d %d\n", boat_id, i);
                berth[i].boat_is_coing = 1;
                break;
            }
        }
    }
    if(boat[boat_id].status == 1 && boat[boat_id].pos != -1){
        if(boat[boat_id].startzhen == 0) boat[boat_id].startzhen = nowzhen;
        if(boat[boat_id].startzhen + 150 < nowzhen) //100 9.5w分 50 8w分 200 9.1w分
        {
            printf("ship %d\n",boat_id);
            berth[boat[boat_id].pos].now_items = 0;
            berth[boat[boat_id].pos].boat_is_coing = 0;
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

