#ifndef TARJAN_H
#define TARJAN_H

#include <bits/stdc++.h>

typedef long long ll;

vector<vector<ll>> g;
vector<ll> low, d;
ll timer = 0;
stack<ll> s;
vector<vector<ll>> components;

void dfs(int curr, int parent);