//#define DEBUG
#include <bits/stdc++.h>

using namespace std;

#define fi first
#define sc second 
#define pb push_back


#ifdef DEBUG
    #define dbg(s) {s;}
    #define PRELUDE
#endif

#ifndef DEBUG 
    #define PRELUDE { ios :: sync_with_stdio(false); cin.tie(0); cout.tie(0); }
    #define dbg(s)
    #define endl "\n"
#endif

#define int ll
#define i32 int32_t

#define all(v) (v).begin(),(v).end()

typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< ll, ll > pll;

const ll mod = 1e9 + 7;
const int maxn = 1e5 + 7;

const ll inf = 1e8;

const int alpha = 100; //max
const int beta = 1;    //sum
const int gama = 10;  //avg
const int delta = 400;    //median
const int rho = 1000;    //-time
const int sigm = 0; // -min

int b,l,d;
int scores[maxn];
int reg[maxn];
int ships[maxn];
vector<int> books[maxn];
vector<int> books2[maxn];

int counti[maxn];
int daysi[maxn];
int curscore;
bool used[maxn];
vector<pii> libs;
vector<vector<int>> ans;

void addLib(int idx)
{
    if(reg[idx]<0)
        return;
    int days = d - reg[idx];
    int maxBooks = days*ships[idx];
    if(maxBooks<=0)
        return;
    vector<int> sc;
    for(auto x : books[idx])
    {
        if(used[x])
            continue;
        sc.pb(scores[x]);
    }
    sort(all(sc));
    reverse(all(sc));
    int maxi = 0;
    int sum = 0;
    int g = sc.size();
    g = min(g,maxBooks);
    int mini = inf;
    for(int i=0;i<g;i++)
    {
        maxi=max(maxi,sc[i]);
        mini = min(mini,sc[i]);
        sum+=sc[i];
    }
    int median = sc[g/2];
    if(sum==0)
        return;
    int avg = sum/reg[idx];
    int scorex = alpha*maxi + beta*sum + gama*avg + delta*median - rho*reg[idx] - sigm*mini;
    dbg(cout<<idx<<"->"<<scorex<<endl;);
    libs.pb({scorex,idx});
}

int findMax()
{
    int x = libs[0].sc;
    int cur = libs[0].fi;

    for(int i=1;i<libs.size();i++)
    {
        if(cur< libs[i].fi)
        {
            cur = libs[i].fi;
            x = libs[i].sc;
        }
    }

    return x;
}

void relaxLibrary(int idx)
{
    int days = d - reg[idx];
    d-= reg[idx];
    reg[idx] = -1-reg[idx];
    daysi[idx] = days;
    int maxBooks = days*ships[idx];
    vector<pii> sc;
    for(auto x : books[idx])
    {
        if(used[x])
            sc.pb({0,x});
        else
            sc.pb({scores[x],x});
    }
    sort(all(sc));
    reverse(all(sc));
    int g = sc.size();
    g = min(g,maxBooks);
    vector<int> xz;
    xz.pb(idx);
    xz.pb(g);
    ans.pb(xz);
    xz.clear();
    int sum = 0;
    for(int i=0;i<g;i++)
    {
        xz.pb(sc[i].sc);
        books2[idx].pb(sc[i].sc);
        used[sc[i].sc]= true;
        counti[sc[i].sc]++;
        sum+=sc[i].fi;
    }
    curscore+=sum;
    ans.pb(xz);
}

int tryChange(int idx)
{
    if(reg[idx]<0)
        return 0;

    int u,v,w;
    for(int i=0;i<l;i++)
    {
        if(reg[i]>=0)
            continue;
        u = -reg[i] - 1;
        if(u<reg[idx])
            continue;
        v = daysi[i];
        int scorechan = 0;
        int maxBooks = v*ships[idx];
        vector<pii> sc;
        for(auto x : books[idx])
        {
            if(counti[x]>0)
                sc.pb({0,x});
            else
                sc.pb({scores[x],x});
        }
        sort(all(sc));
        reverse(all(sc));
        int g = sc.size();
        g = min(g,maxBooks);
        vector<int> xz;
        xz.pb(idx);
        xz.pb(g);
        //ans.pb(xz);
        vector<int> xy;
        //xz.clear();
        int sum = 0;
        for(int j=0;j<g;j++)
        {
            xy.pb(sc[j].sc);
            counti[sc[j].sc]++;
            sum+=sc[j].fi;
        }
        scorechan+=sum;

        
        for(auto x : books2[i])
        {
            if(counti[x]>1)
                continue;
            else
                scorechan -=scores[x];
        }

        if(scorechan>0)
        {
            ans.pb(xz);
            ans.pb(xy);
            reg[i] = 1;
            reg[idx] = -1;
            return scorechan;
        }
        
        for(int j=0;j<g;j++)
        {
            counti[sc[j].sc]--;
        }
        //ans.pb(xz);
    }
    return 0;
}

void solve()
{
    //Input
    cin>>b>>l>>d;
    for(int i=0;i<b;i++)
    {
        cin>>scores[i];
    }

    int n,x;
    for(int i=0;i<l;i++)
    {
        cin>>n>>reg[i]>>ships[i];
        for(int j=0;j<n;j++)
        {
            cin>>x;
            books[i].pb(x);
        }
    }
    dbg(cout<<"inputEnd"<<endl;);
    //Heuristic Start
    memset(used,false,sizeof(used));
    memset(counti,0,sizeof(counti));
    memset(daysi,0,sizeof(daysi));
    //curscore = 0;
    int libidx;
    while(true)
    {
        dbg(cout<<"New Iteration"<<endl;);
        libs.clear();
        for(int i=0;i<l;i++)
        {
            addLib(i);
        }
        if(libs.size()==0)
            break;
        libidx = findMax();
        //curscore+=libs[libidx].fi;
        relaxLibrary(libidx);
    }

    n = ans.size();
    n/=2;

    for(int i=0;i<l;i++){
        x = tryChange(i);
        if(x>0)
        {
            curscore+=x;
            break;
        }
    }

	//cout<<curscore<<endl;
    ofstream outfile;
    outfile.open("scores.txt", ios::app);
    outfile<<curscore<<endl;
    outfile.close();
    
    cout<<n<<endl;
    for(int i=0;i<ans.size();i+=2)
    {
        x = ans[i][0];
        if(reg[x]>=0)
            continue;
        for(auto y:ans[i])
        {
            cout<<y<<" ";
        }
        cout<<endl;
        for(auto y:ans[i+1])
        {
            cout<<y<<" ";
        }
        cout<<endl;
    }
}

i32 main() {
    //freopen("<file>.in", "r", stdin);
    //freopen("<file>.out", "w", stdout);
    
    PRELUDE;
    int t = 1;
    //cin>>t;
    while(t--)
        solve();
}
