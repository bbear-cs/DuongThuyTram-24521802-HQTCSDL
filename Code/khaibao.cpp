#include<bits/stdc++.h>
using namespace std;
const int BAC = 3;

struct SinhVien{
    int mssv;
    string hoten;
    string gioitinh;
};

struct Key {
    int mssv;
    int key;
};

struct node {
    vector<Key> data;
    vector<node*> con;
    bool la;
};
typedef node* Node;

Node create(bool la) {
    Node tmp = new node;
    tmp->la = la;
    return tmp;
}

void xoa(Node &root);

void tach(Node cha, int i) {
    Node y = cha->con[i];
    Node z = create(y->la);
    Key giua = y->data[1];
    z->data.push_back(y->data[2]);

    if(!y->la) {
        z->con.push_back(y->con[2]);
        z->con.push_back(y->con[3]);
    }
    y->data.erase(y->data.begin() + 1, y->data.end());
    if(!y->la) y->con.erase(y->con.begin() + 2, y->con.end());

    cha->con.insert(cha->con.begin() + i + 1, z);
    cha->data.insert(cha->data.begin() + i, giua);
}

void insert(Node &root, Key x) {
    if(root == NULL) {
        root = create(true);
        root->data.push_back(x);
        return;
    }

    if((int)root->data.size() == BAC - 1) {
        if(root->la) {
            root->data.push_back(x);
            int i = (int)root->data.size() - 2;
            while(i >= 0 && root->data[i].mssv > x.mssv) {
                root->data[i + 1] = root->data[i];
                i--;
            }
            root->data[i + 1] = x;

            if((int)root->data.size() == BAC) {
                Node trai = create(true);
                Node phai = create(true);

                trai->data.push_back(root->data[0]);
                phai->data.push_back(root->data[2]);

                Key giua = root->data[1];

                root->data.clear();
                root->con.clear();
                root->la = false;

                root->data.push_back(giua);
                root->con.push_back(trai);
                root->con.push_back(phai);
            }
            return;
        }
        else {
            Node gocCu = root;
            Node gocMoi = create(false);
            gocMoi->con.push_back(gocCu);
            root = gocMoi;
            tach(root, 0);
        }
    }

    Node p = root;

    while(!p->la) {
        int i = (int)p->data.size() - 1;
        while(i >= 0 && x.mssv < p->data[i].mssv) {
            i--;
        }
        i++;
        if((int)p->con[i]->data.size() == BAC - 1) {
            p->con[i]->data.push_back(x);
            int j = (int)p->con[i]->data.size() - 2;
            while(j >= 0 && p->con[i]->data[j].mssv > x.mssv) {
                p->con[i]->data[j + 1] = p->con[i]->data[j];
                j--;
            }
            p->con[i]->data[j + 1] = x;
            if((int)p->con[i]->data.size() == BAC) tach(p, i);
            return;
        }

        p = p->con[i];
    }

    p->data.push_back(x);
    int i = (int)p->data.size() - 2;
    while(i >= 0 && p->data[i].mssv > x.mssv) {
        p->data[i + 1] = p->data[i];
        i--;
    }
    p->data[i + 1] = x;
}

int find(Node root, int mssv) {
    if(root == NULL) return -1;

    int i = 0;
    while(i < (int)root->data.size() && mssv > root->data[i].mssv) {
        i++;
    }

    if(i < (int)root->data.size() && mssv == root->data[i].mssv) return root->data[i].key;
    if(root->la) return -1;
    return find(root->con[i], mssv);
}

bool xoaSV(vector<SinhVien> &ds, Node &root, int mssv) {
    int vt = -1;
    for(int i = 0; i < (int)ds.size(); i++) {
        if(ds[i].mssv == mssv) {
            vt = i;
            break;
        }
    }

    if(vt == -1) return false;
    ds.erase(ds.begin() + vt);
    xoa(root);
    root = NULL;

    for(int i = 0; i < (int)ds.size(); i++) {
        Key a;
        a.mssv = ds[i].mssv;
        a.key = i;
        insert(root, a);
    }
    return true;
}

void xoa(Node &root) {
    if(root == NULL) return;
    if(!root->la) {
        for(int i = 0; i < (int)root->con.size(); i++) {
            xoa(root->con[i]);
        }
    }
    delete root;
    root = NULL;
}