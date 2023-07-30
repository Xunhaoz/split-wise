#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

int level;
vector<int> before_splitting, original;
vector<vector<int>> result;
vector<pair<int, int>> pos_sorted_transaction, neg_sorted_transaction;

vector<int> update_data(vector<int> v) {
    vector<int> res;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] == 1) {
            before_splitting[i] = 0;
            res.push_back(i);
        }
    }

    return res;
}

bool can_pay(vector<int> v) {
    int balance = 0;
    int all_zero = 1;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] == 1 && before_splitting[i] != 0) {
            all_zero = 0;
            balance += before_splitting[i];
        }
    }

    if (!(balance || all_zero)) return true;
    return false;
}


void recur(int deep, int last_one, int max_length, vector<int> v) {
    int all_zero = 1;
    for (auto i: before_splitting) {
        if (i != 0) {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) return;

    if (deep == 0) {
        if (can_pay(v)) result.push_back(update_data(v));
        return;
    }

    for (int i = last_one; i < max_length; i++) {
        v[i] = 1;
        recur(deep - 1, i + 1, max_length, v);
        v[i] = 0;
    }
}


int main() {
    cin >> level;
    before_splitting = vector<int>(level, 0);

    for (auto &i: before_splitting) cin >> i;

    original = before_splitting;

    vector<int> v(level, 0);
    for (int i = 2; i <= level; i++) {
        recur(i, 0, level, v);
    }

    for (auto &transaction: result) {
        pos_sorted_transaction.clear();
        neg_sorted_transaction.clear();

        for (auto member: transaction) {
            if (original[member] > 0)
                pos_sorted_transaction.push_back({original[member], member});
            else
                neg_sorted_transaction.push_back({original[member], member});
        }

        while (!pos_sorted_transaction.empty() || !neg_sorted_transaction.empty()) {
            pos_sorted_transaction.erase(remove_if(pos_sorted_transaction.begin(), pos_sorted_transaction.end(),
                                                   [&](const pair<int, int> stopPoint) -> bool {
                                                       return stopPoint.first == 0;
                                                   }), pos_sorted_transaction.end());

            neg_sorted_transaction.erase(remove_if(neg_sorted_transaction.begin(), neg_sorted_transaction.end(),
                                                   [&](const pair<int, int> stopPoint) -> bool {
                                                       return stopPoint.first == 0;
                                                   }), neg_sorted_transaction.end());


            sort(pos_sorted_transaction.rbegin(), pos_sorted_transaction.rend());
            sort(neg_sorted_transaction.begin(), neg_sorted_transaction.end());


            int pair_length = min(pos_sorted_transaction.size(), neg_sorted_transaction.size());

            for (int i = 0; i < pair_length; i++) {
                int transaction = min(abs(neg_sorted_transaction[i].first), abs(pos_sorted_transaction[i].first));
                cout << neg_sorted_transaction[i].second << " should pay " << pos_sorted_transaction[i].second << " $"
                     << transaction << endl;
                neg_sorted_transaction[i].first += transaction;
                pos_sorted_transaction[i].first -= transaction;
            }
        }
    }

    return 0;
}
