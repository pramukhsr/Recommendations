// Pramukh Sreerama: CS 251 UIC Spring 2021 Professor Reckinger
// This program will take user data on various mediums and display that data
// for the user while also recommending new items.
// Creative Component: Type "missing" as a command while logged in to show
// what items are yet to be rated

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ourvector.h"

using namespace std;

struct user{
    string name;
    ourvector<string> title;
    ourvector<int> rating;
};

struct Rating{
    string title;
    double averageRatings;
};

struct topRating{
    string title;
    double averageRatings;
};

struct Advanced{
    string name;
    int dotProduct;
};

struct advacedBasic{
    string title;
    int averageRatings;
};
// This function takes in the items file and loads items into the vector of 
// structs for user.
void loadItemsFile(string itemsFile, ourvector<string>& titles,
ourvector<Rating>& allRatings) {
    ifstream infile(itemsFile);
    string line;

    if (!infile.good()) {
        cout << "**file not found" << endl;
    }

    cout << "Reading items file..." << endl;

    while (getline(infile, line)) {
        titles.push_back(line);
        Rating currentRating;
        currentRating.title = line;
        allRatings.push_back(currentRating);
        cout << line << endl;
    }
    cout << endl;
}
// This function takes in the ratings file and loads ratings and users into the 
// vector of structs for user.
void loadRatingsFile(string ratingsFile, ourvector<user>& allUsers,
ourvector<string>& titles) {
    ifstream infile(ratingsFile);
    string line;

    if (!infile.good()) {
        cout << "**file not found" << endl;
    }

    cout << "Reading ratings file..." << endl;

    while (getline(infile, line)) {
        user currentUser;
        cout << line << endl;
        currentUser.name = line;
        int var = 0;
        for (int i = 0; i < titles.size(); i++) {
            infile >> var;
            cout << var << " ";
            currentUser.rating.push_back(var);
        }
        cout << endl;
        infile.ignore(1000000000, '\n');
        allUsers.push_back(currentUser);
    }
    cout << endl;
}
// This is the function that checks for a valid user login while making an
// index to keep track of logged in user.For future use if user is not logged in
// the user index is set to -5. This will come up a lot more in the code.
int userLoggedIn(string username, ourvector<user> allUsers) {
    int y = 0;

    for (int i = 0; i < allUsers.size(); i++) {
        if (username == allUsers[i].name) {
            y = i;
            break;
        } else {
            y = -5;
        }
    }
    if (y != -5) {
        cout << "-----------------------------" << endl << endl << "Success." <<
        endl << "Logged in as: " << username << endl;
        return y;
    } else if (y == -5) {
        cout << "-----------------------------" << endl << endl << 
        "User not found, please try again." << endl << "Logged in as: N/A" <<
        endl;
        return -5;
    }
    return -5;
}
// This function shows the rated item data for the current logged in user.
void showData(ourvector<user> allUsers, int userIndex) {
    cout << allUsers[userIndex].name << "'s Ratings:" << endl;
    int itemCount = allUsers[userIndex].rating.size();
    for (int i = 0; i < itemCount; i++) {
        if (allUsers[userIndex].rating[i] != 0) {
            cout << allUsers[userIndex].title[i] << ", " <<
            allUsers[userIndex].rating[i] << "/5" << endl;
        }
    }
    cout << endl << "Logged in as: " << allUsers[userIndex].name << endl;

}
// This function loops through all corresponding reviews for each user per item
// and finds the average rating per item and pushes into the vector of structs
// called Rating.
void avgRating(ourvector<user> allUsers, ourvector<Rating>& allRatings) {
    double average = 0;
    int x = 0;
    for (int i = 0; i < allUsers[0].rating.size(); i++) {
        int count = 0;
        int total = 0;
        for (int j = 0; j < allUsers.size(); j++) {
            total += allUsers[j].rating[i];
            count++;
        }
        average = static_cast<double>(total)/count;
        allRatings[x].averageRatings = average;
        x++;
    }
}
// This function is used primarily for the basic command to look for which
// ratings the logged in user has not rated and push back those average ratings
// into a new vector of struct called topRating.
void findRatings(ourvector<Rating> allRatings, ourvector<user> allUsers,
int userIndex, ourvector<topRating>& topUserRating) {
    topUserRating.clear();
    for (int i = 0; i < allUsers[userIndex].rating.size(); i++) {
        if (allUsers[userIndex].rating[i] == 0) {
            topRating currentTopRating;
            currentTopRating.title = allRatings[i].title;
            currentTopRating.averageRatings = allRatings[i].averageRatings;
            topUserRating.push_back(currentTopRating);
        }
    }
}
// This function will take the average ratings for unrated items from the
// current user and sort them in descending order. Then it will display up to
// the top 5 which would finish the basic command.
void sortRatings(ourvector<topRating>& topUserRating) {
    for (int i = 0; i < topUserRating.size(); i++) {
        for (int j = i; j > 0; --j) {
            if (topUserRating[j].averageRatings >
            topUserRating[j-1].averageRatings) {
                auto temp = topUserRating[j];
                topUserRating[j] = topUserRating[j-1];
                topUserRating[j-1] = temp;
            }
        }
    }
    cout << endl << "Basic recommendations:" << endl;
    for (int i = 0; (i < 5 && i < topUserRating.size()); i++) {
        cout << topUserRating[i].title << endl;
    }
}
// This function loops through the user struct comparing logged in user rating
// with all other users and finds the dot product. The dot products are then
// pushed into a new vector of structs called Advanced.
void findRelatedUser(ourvector<user> allUsers, int userIndex,
ourvector<Advanced>& advancedRec) {
    advancedRec.clear();
    for (int i = 0; i < allUsers.size(); i++) {
        int sum = 0;
        int dot = 0;
        string name;
        Advanced currentDot;

        if (i != userIndex) {
            for (int j = 0; j < allUsers[0].rating.size(); j++) {
                dot = allUsers[userIndex].rating[j] * allUsers[i].rating[j];
                sum += dot;
                name = allUsers[i].name;
            }
            currentDot.dotProduct = sum;
            currentDot.name = allUsers[i].name;
            advancedRec.push_back(currentDot);
        }
    }
}
// This function sorts the Advanced struct by highest to lowest and returns
// the name of the user with the highest dot product. This user should be
// compared to the current logged in user for the advanced command.
string sortAdvanced(ourvector<Advanced>& advancedRec) {

    int tempMax;
    for (int i = 0; i < advancedRec.size(); i++) {
        tempMax = i;
        for (int j = i + 1; j < advancedRec.size(); j++) {
            if (advancedRec[j].dotProduct > advancedRec[tempMax].dotProduct) {
                tempMax = j;
            }
        }

        int temp = advancedRec[tempMax].dotProduct;
        advancedRec[tempMax].dotProduct = advancedRec[i].dotProduct;
        advancedRec[i].dotProduct = temp;

        string temp1 = advancedRec[tempMax].name;
        advancedRec[tempMax].name = advancedRec[i].name;
        advancedRec[i].name = temp1;
    }

    string topname = advancedRec[0].name;
    return topname;
}
// This function takes the name of the comparable user for advanced and returns
// that persons index from the original user struct for easier comparison.
int findNameIndex(string topName, ourvector<user> allUsers) {
    int index;
    for (int i = 0; i < allUsers.size(); i++) {
        if (allUsers[i].name == topName) {
            index = i;
        }
    }
    return index;
}
// This function takes the comparable user for advanced and pushes all their
// ratings and titles into a new struct called advacedBasic.
void basicForAdvanced(ourvector<user> allUsers, int index,
ourvector<advacedBasic>& topAdvancedRating) {
    topAdvancedRating.clear();
    for (int i = 0; i < allUsers[index].rating.size(); i++) {
        advacedBasic currentTopRating;
        currentTopRating.title = allUsers[index].title[i];
        currentTopRating.averageRatings = allUsers[index].rating[i];
        topAdvancedRating.push_back(currentTopRating);
    }
}
// This function will sort the comparable user's rating data from highest to
// lowest while keepng insertion order for ties.
void sortAdvacedBasic(ourvector<advacedBasic>& topAdvancedRating) {

    for (int i = 0; i < topAdvancedRating.size(); i++) {
        for (int j = i; j > 0; --j) {
            if (topAdvancedRating[j].averageRatings >
            topAdvancedRating[j-1].averageRatings) {
                auto temp = topAdvancedRating[j];
                topAdvancedRating[j] = topAdvancedRating[j-1];
                topAdvancedRating[j-1] = temp;
            }
        }
    }
}
// This is the final function related to advanced. It loops through the Logged
// in user's ratings and if there is a 0 and the corresponding title matches
// with the comparable user's top items the the top 5 of those items are
// displayed
void compareAdvanced(ourvector<user> allUsers, int userIndex,
ourvector<advacedBasic> topAdvancedRating, ourvector<string> Advanced) {
    for (int i = 0; i < topAdvancedRating.size(); i++) {
        for (int j = 0; j < allUsers[userIndex].rating.size(); j++) {
            if (allUsers[userIndex].rating[j] == 0 &&
            allUsers[userIndex].title[j] == topAdvancedRating[i].title) {
                Advanced.push_back(topAdvancedRating[i].title);
            }
        }
    }
    cout << endl << "Advanced recommendations:" << endl;
    for (int i = 0; i < 5 && i < Advanced.size(); i++) {
        cout << Advanced[i] << endl;
    }
    cout << endl << "Logged in as: " << allUsers[userIndex].name << endl;
}
// This function will print users and items to console.
void printCommand(ourvector<user> allUsers, int userIndex) {
    string line;
    cin >> line;

    if (line == "users") {
        cout << "-----------------------------" << endl << endl;
        for (int i = 0; i < allUsers.size(); i++) {
            cout << allUsers[i].name << endl;
        }
    } else if (line == "items") {
        cout << "-----------------------------" << endl << endl;
        int count = 0;
        int itemCount = allUsers[1].title.size();
        for (int i = 0; i < itemCount; i++) {
            cout << count << ". " << allUsers[1].title[i] << endl;
            count++;
        }
    }
    if (userIndex == -5) {
        cout << endl << "Logged in as: N/A" << endl;
    } else {
        cout << endl << "Logged in as: " << allUsers[userIndex].name << endl;
    }
}
// This function will add users to the user struct and allocate and empty vector
// for the new user for items and a copy vector of corresponding titles.
void addUser(ourvector<user>& allUsers, string username, int& userIndex) {
    user currentUser;
    currentUser.name = username;
    userIndex = allUsers.size();
    for (int i = 0; i < allUsers[1].rating.size(); i++) {
        currentUser.rating.push_back(0);
        currentUser.title.push_back(allUsers[1].title[i]);
    }
    allUsers.push_back(currentUser);

    cout << "Logged in as: " << allUsers[userIndex].name << endl;
}
// This function will take in new ratings for existing or new users and input
// those ratings in the indexed location.
void addRating(ourvector<user>& allUsers, int userIndex, int itemNumber) {

    cout << "Add the rating for " << allUsers[userIndex].title[itemNumber] <<
    ":";
    int newRating;
    cin >> newRating;
    allUsers[userIndex].rating[itemNumber] = newRating;
    cout << endl << "Logged in as: " << allUsers[userIndex].name << endl;
}
// This houses the other two add functions for better function decomposition.
void addCommand(ourvector<user>& allUsers, string addLine, int& userIndex) {
    if (addLine == "user") {
        string username;
        cout << "-----------------------------" << endl << endl <<
        "Please enter the username: ";
        cin.ignore();
        getline(cin, username);
        addUser(allUsers, username, userIndex);
    }
    if (addLine == "rating") {
        if (userIndex != -5) {
            cout << "-----------------------------" << endl << endl <<
            "Item number to add/change rating: ";
            int itemNumber;
            cin >> itemNumber;
            addRating(allUsers, userIndex, itemNumber);
        } else {
            cout << "-----------------------------" << endl << endl <<
            "To complete the operation, you must first login." << endl <<
            "Type \"login username\"." << endl << endl << "Logged in as: N/A" <<
            endl;
        }
    }
}
// This is my creative component. When typing in the command "missing", This
// function will look at what the logged in user has not rated and display those
// titles with their corresponding indexes to the console.
void toWatch(ourvector<user>& allUsers, int userIndex) {

    cout << "Items to rate: " << endl;
    for (int i = 0; i < allUsers[userIndex].rating.size(); i++) {
        if (allUsers[userIndex].rating[i] == 0) {
            cout << i << ". " << allUsers[userIndex].title[i] << endl;
        }
    }
    cout << endl;
}

int main() {
    ourvector<user> allUsers; ourvector<string> titles;
    ourvector<Rating> allRatings; ourvector<topRating> topUserRating;
    ourvector<Advanced> advancedRec; ourvector<advacedBasic> topAdvancedRating;
    ourvector<string> Advanced;

    cout << "To start the app, load the data." << endl;
    cout << "Type \"load itemsFile ratingsFile\" and press enter." << endl <<
    endl;
    string command;
    int userIndex = -5;

    while (true) {
        cout << "Enter command or # to quit: ";
        cin >> command;
        cout << endl;

        if (command == "#") {
            cout << "-----------------------------\n" << endl << endl <<
            "Thank you for using the Recommendations app!" << endl;
            break;
        }
        if (command == "load") {
            string itemsFile, ratingsFile;
            cin >> itemsFile >> ratingsFile;

            loadItemsFile(itemsFile, titles, allRatings);
            loadRatingsFile(ratingsFile, allUsers, titles);

            for (user& currentUser : allUsers) {
                currentUser.title = titles;
            }
            cout << "Welcome to the Recommendations App!" << endl;
        }
        if (command == "login") {
            string username;
            cin.ignore();
            getline(cin, username);
            userIndex = userLoggedIn(username, allUsers);
        }
        if (command == "show") {
            cout << "-----------------------------" << endl << endl;
            if(userIndex != -5){
                showData(allUsers, userIndex);
            }
            else{
                cout << "To complete the operation, you must first login." <<
                endl << "Type \"login username\"." << endl << endl <<
                "Logged in as: N/A" << endl;
            }
        }
        if (command == "print") {
            printCommand(allUsers, userIndex);
        }
        if (command == "add") {
            string addLine;
            cin >> addLine;
            addCommand(allUsers, addLine, userIndex);
        }
        if (command == "basic") {
            cout << "-----------------------------" << endl;
            if (userIndex != -5) {
                avgRating(allUsers, allRatings);
                findRatings(allRatings, allUsers, userIndex, topUserRating);
                sortRatings(topUserRating);
                cout << endl << "Logged in as: " << allUsers[userIndex].name <<
                endl;
            } else {
                cout << endl <<
                "To complete the operation, you must first login." << endl <<
                "Type \"login username\"." << endl << endl <<
                "Logged in as: N/A" << endl;
            }
        }
        if (command == "advanced") {
            cout << "-----------------------------" << endl;
            if (userIndex != -5) {
                findRelatedUser(allUsers, userIndex, advancedRec);
                string topName;
                topName = sortAdvanced(advancedRec);
                int index = findNameIndex(topName, allUsers);
                avgRating(allUsers, allRatings);
                basicForAdvanced(allUsers, index, topAdvancedRating);
                sortAdvacedBasic(topAdvancedRating);
                compareAdvanced(allUsers, userIndex, topAdvancedRating,
                Advanced);
            } else {
                cout << endl <<
                "To complete the operation, you must first login." << endl <<
                "Type \"login username\"." << endl << endl <<
                "Logged in as: N/A" << endl;
            }
        }
        if (command == "missing") {
            toWatch(allUsers, userIndex);
        }
    }
    return 0;
}