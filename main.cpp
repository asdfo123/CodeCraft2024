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
}boat[10];

struct Item{
    int x, y;
    int startzhen;
    int val;
    bool operator==(const Item& other) const {
        return x == other.x && y == other.y && startzhen == other.startzhen;
    }
};
int money, boat_capacity, id;
char ch[N][N];
vector<Item> items;

void Init()
{
    for(int i = 1; i <= n; i ++)
        scanf("%s", ch[i] + 1);
    for(int i = 0; i < berth_num; i ++)
    {
        int idd;
        scanf("%d", &idd);
        scanf("%d%d%d%d", &berth[idd].x, &berth[idd].y, &berth[idd].transport_time, &berth[idd].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        items.push_back(Item{x, y, id, val});
    }
    for(int i = 0; i < robot_num; i ++)
    {
        int sts;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
    }
    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[100];
    scanf("%s", okk);
    return id;
}

const int INF = INT_MAX;

// 0: right, 1: left, 2: up, 3: down
const int dx[] = {0, 0, -1, 1};
const int dy[] = {1, -1, 0, 0};
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

//BFS: return a 2D vector of shortest distance from start to each point.
vector<vector<int>> bfs(const Point& start) {
    vector<vector<int>> dist(N, vector<int>(N, INF));
    queue<Point> q;
    q.push(start);
    dist[start.x][start.y] = 0;

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (nx >= 0 && nx < N && ny >= 0 && ny < N && ch[nx][ny] == '.' && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                q.push(Point(nx, ny));
            }
        }
    }

    return dist;
}


int findNextMove(int robot_id) {
    vector<vector<int>> dist = bfs(Point(robot[robot_id].x, robot[robot_id].y));
    int min_distance = INF;
    Point targetPos(0, 0);
    for (const Item& item : items) {
        int distance = dist[item.x][item.y];
        if (distance < min_distance) {
            min_distance = distance;
            targetPos = Point(item.x, item.y);
        }
    }

    int x = targetPos.x;
    int y = targetPos.y;
    int dir1 = -1;
    while (dist[x][y] > 1) {
        for (int dir = 0; dir < 4; dir++) {
            int nx = x - dx[dir];
            int ny = y - dy[dir];
            if (nx >= 0 && nx < N && ny >= 0 && ny < N && dist[nx][ny] == dist[x][y] - 1) {
                x = nx;
                y = ny;
                break;
            }
        }
    }
    if(dist[x][y] == 1){
        for(int dir = 0; dir < 4; dir++){
            int nx = x - dx[dir];
            int ny = y - dy[dir];
            if(nx == robot[robot_id].x && ny == robot[robot_id].y){
                dir1 = dir;
                break;
            }
        }
    }

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
void handle_robot(int robot_id)
{
//    if(robot[robot_id].status == 0) //is_recovering
//        return;
//    int dir = findNextMove(robot_id);
    printf("move %d %d\n", robot_id, rand()%4);
}
int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input();
        handle_item(zhen);
        for(int i = 0; i < robot_num; i++){
            handle_robot(i);
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
