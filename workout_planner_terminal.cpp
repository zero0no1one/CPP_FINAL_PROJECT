#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

const string RESET   = "\033[0m";
const string BLACK   = "\033[30m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
const string BOLD    = "\033[1m";

struct UserInfo {
    string name;
    string age;
    string goal;        
    string equipment;   
};

struct WorkoutDay {
    string time;
    string type;        // cardio, strength, flexibility, hiit, mixed
    bool completed;
};

// Structure for Progress Record (equivalent to dictionary in list)
struct ProgressRecord {
    string date;
    string day;
    string exercises;
    string duration;
    string notes;
};

// Structure to hold all data for one user
struct UserData {
    UserInfo userInfo;
    map<string, WorkoutDay> workoutSchedule;    // Dictionary: day -> WorkoutDay
    vector<ProgressRecord> progressData;        // List of progress records
};

// ============================================
// GLOBAL VARIABLES
// ============================================

string currentUser;                              // Currently logged-in user
map<string, UserData> allUsers;                  // Dictionary storing all users
map<string, vector<string>> exercisesDB;         // Exercise database

// ============================================
// FUNCTION DECLARATIONS
// ============================================

// Utility functions
void initializeExerciseDB();
string getCurrentDate();
void clearScreen();
void pauseScreen();
string toLower(string str);
string capitalize(string str);
void printHeader(const string& title);
void printBox(const string& text, const string& color);

// Data persistence functions
void saveAllUsersData();
void loadAllUsersData();
void saveCurrentUserData();

// Menu functions
void showLoginScreen();
void loginUser();
void createNewUser();
void logoutUser();
void showMainMenu();

// Feature functions
void setGoals();
void createSchedule();
void suggestWorkouts();
void recordProgress();
void viewProgress();

// Exit function
void exitProgram();

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    // Initialize exercise database
    initializeExerciseDB();
    
    // Load existing data
    loadAllUsersData();
    
    // Show login screen
    showLoginScreen();
    
    return 0;
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

void initializeExerciseDB() {
    // Initialize exercises database (equivalent to Python exercises_db dictionary)
    exercisesDB["cardio"] = {"Running", "Cycling", "Jumping Jacks", "Burpees", "Mountain Climbers"};
    exercisesDB["strength"] = {"Push-ups", "Squats", "Lunges", "Plank", "Dumbbell Curls"};
    exercisesDB["flexibility"] = {"Yoga", "Stretching", "Pilates", "Foam Rolling"};
    exercisesDB["hiit"] = {"Sprint Intervals", "Tabata", "Box Jumps", "Kettlebell Swings"};
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday;
    
    return ss.str();
}

void clearScreen() {
    // Clear terminal screen - works on Windows and Unix/Linux/Mac
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << endl << CYAN << "Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string capitalize(string str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
    }
    return str;
}

void printHeader(const string& title) {
    // Print colored header box for terminal
    int width = 48;
    cout << endl;
    cout << BOLD << BLUE;
    cout << "+" << string(width, '-') << "+" << endl;
    
    // Center the title
    int padding = (width - title.length()) / 2;
    cout << "|" << string(padding, ' ') << CYAN << title << BLUE 
         << string(width - padding - title.length(), ' ') << "|" << endl;
    
    cout << "+" << string(width, '-') << "+" << RESET << endl;
    cout << endl;
}

void printBox(const string& text, const string& color) {
    cout << color << "[ " << text << " ]" << RESET << endl;
}

// ============================================
// DATA PERSISTENCE FUNCTIONS
// ============================================

void saveAllUsersData() {
    ofstream file("workout_data.txt");
    
    if (file.is_open()) {
        // Save number of users
        file << allUsers.size() << endl;
        
        // Loop through all users (Control structure: for loop with iterator)
        for (auto it = allUsers.begin(); it != allUsers.end(); ++it) {
            string username = it->first;
            UserData data = it->second;
            
            // Save username
            file << username << endl;
            
            // Save user info
            file << data.userInfo.name << endl;
            file << data.userInfo.age << endl;
            file << data.userInfo.goal << endl;
            file << data.userInfo.equipment << endl;
            
            // Save workout schedule size
            file << data.workoutSchedule.size() << endl;
            
            // Loop through workout schedule
            for (auto sit = data.workoutSchedule.begin(); sit != data.workoutSchedule.end(); ++sit) {
                file << sit->first << endl;            // Day name
                file << sit->second.time << endl;      // Time
                file << sit->second.type << endl;      // Type
                file << sit->second.completed << endl; // Completed status
            }
            
            // Save progress data size
            file << data.progressData.size() << endl;
            
            // Loop through progress data
            for (const ProgressRecord& record : data.progressData) {
                file << record.date << endl;
                file << record.day << endl;
                file << record.exercises << endl;
                file << record.duration << endl;
                file << record.notes << endl;
            }
        }
        
        file.close();
        cout << GREEN << "Data saved successfully!" << RESET << endl;
    } else {
        cout << RED << "Error: Could not save data!" << RESET << endl;
    }
}

void loadAllUsersData() {
    ifstream file("workout_data.txt");
    
    if (file.is_open()) {
        int numUsers;
        file >> numUsers;
        file.ignore(); // Ignore newline
        
        // Loop through all users
        for (int i = 0; i < numUsers; i++) {
            string username;
            getline(file, username);
            
            UserData data;
            
            // Load user info
            getline(file, data.userInfo.name);
            getline(file, data.userInfo.age);
            getline(file, data.userInfo.goal);
            getline(file, data.userInfo.equipment);
            
            // Load workout schedule
            int scheduleSize;
            file >> scheduleSize;
            file.ignore();
            
            for (int j = 0; j < scheduleSize; j++) {
                string day;
                getline(file, day);
                
                WorkoutDay wd;
                getline(file, wd.time);
                getline(file, wd.type);
                file >> wd.completed;
                file.ignore();
                
                data.workoutSchedule[day] = wd;
            }
            
            // Load progress data
            int progressSize;
            file >> progressSize;
            file.ignore();
            
            for (int j = 0; j < progressSize; j++) {
                ProgressRecord record;
                getline(file, record.date);
                getline(file, record.day);
                getline(file, record.exercises);
                getline(file, record.duration);
                getline(file, record.notes);
                
                data.progressData.push_back(record);
            }
            
            allUsers[username] = data;
        }
        
        file.close();
        cout << GREEN << "Data loaded successfully!" << RESET << endl;
    }
}

void saveCurrentUserData() {
    // Conditional: only save if there's a current user
    if (!currentUser.empty()) {
        saveAllUsersData();
    }
}

// ============================================
// LOGIN SCREEN FUNCTIONS
// ============================================

void showLoginScreen() {
    int choice;
    
    do {
        clearScreen();
        
        // Main title header
        cout << BOLD << CYAN;
        cout << "    ╔══════════════════════════════════════════════════╗" << endl;
        cout << "    ║      WORKOUT SCHEDULE PLANNER AND TRACKER        ║" << endl;
        cout << "    ║                  (C++ Terminal)                   ║" << endl;
        cout << "    ╚══════════════════════════════════════════════════╝" << RESET << endl;
        cout << endl;
        
        cout << WHITE << "Welcome! Please Login or Create Account" << RESET << endl;
        cout << endl;
        
        // Display existing users
        if (!allUsers.empty()) {
            cout << YELLOW << "--- Existing Users ---" << RESET << endl;
            int userNum = 1;
            for (auto it = allUsers.begin(); it != allUsers.end(); ++it) {
                cout << "  " << CYAN << userNum++ << ". " << RESET << it->first << endl;
            }
            cout << endl;
        }
        
        // Menu box
        cout << BOLD << BLUE << "╔══════════════════════════════════════╗" << RESET << endl;
        cout << BOLD << BLUE << "║" << YELLOW << "            MAIN MENU               " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "╠══════════════════════════════════════╣" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  1. Login (Existing User)          " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  2. Create New Account             " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  3. Exit                           " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "╚══════════════════════════════════════╝" << RESET << endl;
        cout << endl;
        cout << BOLD << "Enter choice: " << RESET;
        cin >> choice;
        
        // Conditional: switch statement for menu selection
        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                createNewUser();
                break;
            case 3:
                exitProgram();
                return;
            default:
                cout << RED << "Invalid choice!" << RESET << endl;
                pauseScreen();
        }
    } while (choice != 3);
}

void loginUser() {
    string username;
    
    clearScreen();
    printHeader("LOGIN");
    
    // Conditional: check if users exist
    if (allUsers.empty()) {
        cout << YELLOW << "No users found! Please create an account first." << RESET << endl;
        pauseScreen();
        return;
    }
    
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, username);
    
    // Conditional: check if user exists
    if (allUsers.find(username) != allUsers.end()) {
        currentUser = username;
        cout << endl;
        cout << GREEN << "Welcome back, " << BOLD << username << RESET << GREEN << "!" << RESET << endl;
        pauseScreen();
        showMainMenu();
    } else {
        cout << RED << "User not found!" << RESET << endl;
        pauseScreen();
    }
}

void createNewUser() {
    string username;
    
    clearScreen();
    printHeader("CREATE NEW ACCOUNT");
    
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, username);
    
    // Conditional: check if username is empty
    if (username.empty()) {
        cout << RED << "Name cannot be empty!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    // Conditional: check if user already exists
    if (allUsers.find(username) != allUsers.end()) {
        cout << RED << "User already exists! Please login instead." << RESET << endl;
        pauseScreen();
        return;
    }
    
    currentUser = username;
    
    // Initialize new user data
    UserData newUser;
    newUser.userInfo.name = username;
    allUsers[username] = newUser;
    
    saveAllUsersData();
    
    cout << endl;
    cout << GREEN << "Account created for " << BOLD << username << RESET << GREEN << "!" << RESET << endl;
    cout << CYAN << "Please set up your profile." << RESET << endl;
    pauseScreen();
    
    setGoals();
}

void logoutUser() {
    saveCurrentUserData();
    
    cout << endl;
    cout << YELLOW << "You have been logged out successfully!" << RESET << endl;
    pauseScreen();
    
    // Clear current session
    currentUser.clear();
    
    showLoginScreen();
}

// ============================================
// MAIN MENU
// ============================================

void showMainMenu() {
    int choice;
    
    do {
        clearScreen();
        
        // Header
        cout << BOLD << CYAN;
        cout << "    ╔══════════════════════════════════════════════════╗" << endl;
        cout << "    ║           WORKOUT SCHEDULE PLANNER               ║" << endl;
        cout << "    ╚══════════════════════════════════════════════════╝" << RESET << endl;
        cout << endl;
        
        // Display welcome message
        string welcomeText = allUsers[currentUser].userInfo.name.empty() ? 
            "Welcome to Your Fitness Journey!" : 
            "Welcome back, " + allUsers[currentUser].userInfo.name + "!";
        
        cout << BOLD << WHITE << welcomeText << RESET << endl;
        cout << CYAN << "Track your workouts, achieve your goals!" << RESET << endl;
        cout << endl;
        
        // Menu box
        cout << BOLD << BLUE << "╔══════════════════════════════════════╗" << RESET << endl;
        cout << BOLD << BLUE << "║" << YELLOW << "            MAIN MENU               " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "╠══════════════════════════════════════╣" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  1. Set Profile & Goals            " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  2. Create Schedule                " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  3. Suggest Workouts               " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  4. Record Progress                " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  5. View Progress                  " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  6. Logout                         " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "║" << WHITE << "  7. Exit                           " << BLUE << "║" << RESET << endl;
        cout << BOLD << BLUE << "╚══════════════════════════════════════╝" << RESET << endl;
        cout << endl;
        cout << BOLD << "Enter choice: " << RESET;
        cin >> choice;
        
        // Conditional: switch statement
        switch (choice) {
            case 1:
                setGoals();
                break;
            case 2:
                createSchedule();
                break;
            case 3:
                suggestWorkouts();
                break;
            case 4:
                recordProgress();
                break;
            case 5:
                viewProgress();
                break;
            case 6:
                logoutUser();
                return;
            case 7:
                exitProgram();
                return;
            default:
                cout << RED << "Invalid choice!" << RESET << endl;
                pauseScreen();
        }
    } while (choice != 7);
}

// ============================================
// SET GOALS FUNCTION
// ============================================

void setGoals() {
    clearScreen();
    printHeader("SET PROFILE & GOALS");
    
    string name, age, goalChoice, equipChoice;
    string goal, equipment;
    
    // Get name
    cout << CYAN << "Your Name" << RESET << " [" << allUsers[currentUser].userInfo.name << "]: ";
    cin.ignore();
    getline(cin, name);
    if (name.empty()) {
        name = allUsers[currentUser].userInfo.name;
    }
    
    // Get age
    cout << CYAN << "Age" << RESET << " [" << allUsers[currentUser].userInfo.age << "]: ";
    getline(cin, age);
    if (age.empty()) {
        age = allUsers[currentUser].userInfo.age;
    }
    
    // Get fitness goal (Conditional: menu selection)
    cout << endl;
    cout << YELLOW << "Select Fitness Goal:" << RESET << endl;
    cout << "  1. Weight Loss" << endl;
    cout << "  2. Muscle Gain" << endl;
    cout << "  3. General Fitness" << endl;
    cout << "  4. Endurance" << endl;
    cout << "Choice: ";
    getline(cin, goalChoice);
    
    // Conditional: if-else if-else
    if (goalChoice == "1" || toLower(goalChoice) == "weight loss") {
        goal = "weight_loss";
    } else if (goalChoice == "2" || toLower(goalChoice) == "muscle gain") {
        goal = "muscle_gain";
    } else if (goalChoice == "3" || toLower(goalChoice) == "general fitness") {
        goal = "general_fitness";
    } else if (goalChoice == "4" || toLower(goalChoice) == "endurance") {
        goal = "endurance";
    } else {
        goal = allUsers[currentUser].userInfo.goal.empty() ? "general_fitness" : allUsers[currentUser].userInfo.goal;
    }
    
    // Get equipment (Conditional: menu selection)
    cout << endl;
    cout << YELLOW << "Select Available Equipment:" << RESET << endl;
    cout << "  1. None / Bodyweight" << endl;
    cout << "  2. Dumbbells" << endl;
    cout << "  3. Full Gym" << endl;
    cout << "  4. Cardio Machines" << endl;
    cout << "Choice: ";
    getline(cin, equipChoice);
    
    // Conditional: if-else if-else
    if (equipChoice == "1" || toLower(equipChoice) == "none") {
        equipment = "none";
    } else if (equipChoice == "2" || toLower(equipChoice) == "dumbbells") {
        equipment = "dumbbells";
    } else if (equipChoice == "3" || toLower(equipChoice) == "full gym") {
        equipment = "full_gym";
    } else if (equipChoice == "4" || toLower(equipChoice) == "cardio") {
        equipment = "cardio";
    } else {
        equipment = allUsers[currentUser].userInfo.equipment.empty() ? "none" : allUsers[currentUser].userInfo.equipment;
    }
    
    // Conditional: validate name
    if (name.empty()) {
        cout << RED << "Error: Name cannot be empty!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    // Save profile
    allUsers[currentUser].userInfo.name = name;
    allUsers[currentUser].userInfo.age = age;
    allUsers[currentUser].userInfo.goal = goal;
    allUsers[currentUser].userInfo.equipment = equipment;
    
    saveAllUsersData();
    
    cout << endl;
    cout << GREEN << BOLD << "Profile saved successfully!" << RESET << endl;
    pauseScreen();
}

// ============================================
// CREATE SCHEDULE FUNCTION
// ============================================

void createSchedule() {
    clearScreen();
    printHeader("CREATE WORKOUT SCHEDULE");
    
    cout << CYAN << "Select your workout days and times:" << RESET << endl;
    cout << WHITE << "(Enter time for selected days, leave blank to skip)" << RESET << endl;
    cout << endl;
    
    // Days of the week
    vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    map<string, string> selectedDays;
    
    // Loop: for each day (Control structure: for loop)
    for (const string& day : days) {
        string time;
        string existingTime = "";
        
        // Check if day already has a schedule
        if (allUsers[currentUser].workoutSchedule.find(day) != allUsers[currentUser].workoutSchedule.end()) {
            existingTime = allUsers[currentUser].workoutSchedule[day].time;
        }
        
        cout << YELLOW << day << RESET << " [" << (existingTime.empty() ? "--:--" : existingTime) << "]: ";
        cin.ignore();
        getline(cin, time);
        
        // Conditional: if time is entered, add to schedule
        if (!time.empty()) {
            selectedDays[day] = time;
        } else if (!existingTime.empty()) {
            selectedDays[day] = existingTime;
        }
    }
    
    // Get workout type
    cout << endl;
    cout << YELLOW << "Select Preferred Workout Type:" << RESET << endl;
    cout << "  1. Cardio" << endl;
    cout << "  2. Strength" << endl;
    cout << "  3. Flexibility" << endl;
    cout << "  4. HIIT" << endl;
    cout << "  5. Mixed" << endl;
    cout << "Choice: ";
    
    string typeChoice;
    getline(cin, typeChoice);
    
    string workoutType;
    // Conditional: if-else if-else
    if (typeChoice == "1") workoutType = "cardio";
    else if (typeChoice == "2") workoutType = "strength";
    else if (typeChoice == "3") workoutType = "flexibility";
    else if (typeChoice == "4") workoutType = "hiit";
    else workoutType = "mixed";
    
    // Save schedule
    allUsers[currentUser].workoutSchedule.clear();
    
    // Loop: save each selected day
    for (auto it = selectedDays.begin(); it != selectedDays.end(); ++it) {
        WorkoutDay wd;
        wd.time = it->second;
        wd.type = workoutType;
        wd.completed = false;
        allUsers[currentUser].workoutSchedule[it->first] = wd;
    }
    
    saveAllUsersData();
    
    cout << endl;
    cout << GREEN << BOLD << "Schedule saved! " << RESET << GREEN << selectedDays.size() << " workout days set." << RESET << endl;
    pauseScreen();
}

// ============================================
// SUGGEST WORKOUTS FUNCTION
// ============================================

void suggestWorkouts() {
    clearScreen();
    printHeader("SUGGESTED WORKOUTS");
    
    // Conditional: check if user has set profile
    if (allUsers[currentUser].userInfo.equipment.empty()) {
        cout << YELLOW << "⚠ Please set your profile first!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    string equipment = allUsers[currentUser].userInfo.equipment;
    string goal = allUsers[currentUser].userInfo.goal;
    
    // Display header
    cout << CYAN << "Recommended for: " << RESET << BOLD << capitalize(goal) << RESET 
         << CYAN << " | Equipment: " << RESET << BOLD << capitalize(equipment) << RESET << endl;
    cout << MAGENTA << string(50, '-') << RESET << endl;
    cout << endl;
    
    // Determine workout types based on goal (Conditional: if-else if-else)
    vector<string> focusAreas;
    if (goal == "weight_loss") {
        focusAreas = {"cardio", "hiit"};
    } else if (goal == "muscle_gain") {
        focusAreas = {"strength"};
    } else if (goal == "endurance") {
        focusAreas = {"cardio", "strength"};
    } else {
        focusAreas = {"cardio", "strength", "flexibility"};
    }
    
    // Loop: display exercises for each focus area
    for (const string& area : focusAreas) {
        cout << BOLD << YELLOW << area << " EXERCISES:" << RESET << endl;
        cout << YELLOW << string(30, '-') << RESET << endl;
        
        // Get exercises from database
        vector<string> exercises = exercisesDB[area];
        
        // Loop: display each exercise
        for (const string& ex : exercises) {
            cout << WHITE << "  • " << ex << RESET << endl;
        }
        cout << endl;
    }
    
    // Display weekly schedule if available
    if (!allUsers[currentUser].workoutSchedule.empty()) {
        cout << BOLD << GREEN << "YOUR WEEKLY SCHEDULE:" << RESET << endl;
        cout << GREEN << string(30, '-') << RESET << endl;
        
        // Loop: display each scheduled day
        for (auto it = allUsers[currentUser].workoutSchedule.begin(); 
             it != allUsers[currentUser].workoutSchedule.end(); ++it) {
            cout << WHITE << "  " << it->first << ": " << CYAN << it->second.time 
                 << RESET << " - " << MAGENTA << capitalize(it->second.type) << RESET << endl;
        }
        cout << endl;
    }
    
    pauseScreen();
}

// ============================================
// RECORD PROGRESS FUNCTION
// ============================================

void recordProgress() {
    clearScreen();
    printHeader("RECORD WORKOUT PROGRESS");
    
    // Conditional: check if schedule exists
    if (allUsers[currentUser].workoutSchedule.empty()) {
        cout << YELLOW << "⚠ No schedule found! Create a schedule first." << RESET << endl;
        pauseScreen();
        return;
    }
    
    cout << BOLD << WHITE << "Record Today's Workout:" << RESET << endl;
    cout << endl;
    
    // Display available days
    cout << YELLOW << "Select Day:" << RESET << endl;
    int dayNum = 1;
    vector<string> dayList;
    
    // Loop: list all scheduled days
    for (auto it = allUsers[currentUser].workoutSchedule.begin(); 
         it != allUsers[currentUser].workoutSchedule.end(); ++it) {
        cout << "  " << CYAN << dayNum++ << ". " << RESET << it->first << endl;
        dayList.push_back(it->first);
    }
    
    cout << "Choice: ";
    int dayChoice;
    cin >> dayChoice;
    
    // Conditional: validate choice
    if (dayChoice < 1 || dayChoice > static_cast<int>(dayList.size())) {
        cout << RED << "Invalid choice!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    string selectedDay = dayList[dayChoice - 1];
    
    // Get exercises
    cout << endl;
    cout << CYAN << "Exercises Completed (comma-separated): " << RESET;
    cin.ignore();
    string exercises;
    getline(cin, exercises);
    
    // Conditional: check if exercises entered
    if (exercises.empty()) {
        cout << RED << "Error: Please enter at least one exercise!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    // Get duration
    cout << CYAN << "Duration (minutes): " << RESET;
    string duration;
    getline(cin, duration);
    if (duration.empty()) {
        duration = "30";
    }
    
    // Get notes
    cout << CYAN << "Notes: " << RESET;
    string notes;
    getline(cin, notes);
    
    // Create progress record
    ProgressRecord record;
    record.date = getCurrentDate();
    record.day = selectedDay;
    record.exercises = exercises;
    record.duration = duration;
    record.notes = notes;
    
    // Add to progress data (List operation: push_back)
    allUsers[currentUser].progressData.push_back(record);
    
    // Mark workout as completed (Conditional)
    if (allUsers[currentUser].workoutSchedule.find(selectedDay) != 
        allUsers[currentUser].workoutSchedule.end()) {
        allUsers[currentUser].workoutSchedule[selectedDay].completed = true;
    }
    
    saveAllUsersData();
    
    cout << endl;
    cout << GREEN << BOLD << "Progress recorded successfully!" << RESET << endl;
    pauseScreen();
}

// ============================================
// VIEW PROGRESS FUNCTION
// ============================================

void viewProgress() {
    clearScreen();
    printHeader("VIEW PROGRESS");
    
    // Conditional: check if there's progress data
    if (allUsers[currentUser].progressData.empty()) {
        cout << YELLOW << "📭 No progress records yet!" << RESET << endl;
        pauseScreen();
        return;
    }
    
    // Calculate statistics
    int totalWorkouts = allUsers[currentUser].progressData.size();
    int totalDuration = 0;
    
    // Loop: calculate total duration
    for (const ProgressRecord& record : allUsers[currentUser].progressData) {
        // Try to convert duration to integer
        try {
            totalDuration += stoi(record.duration);
        } catch (...) {
            // Ignore invalid duration values
        }
    }
    
    // Display statistics
    cout << BOLD << CYAN << "📈 WORKOUT STATISTICS" << RESET << endl;
    cout << BOLD << BLUE << string(40, '=') << RESET << endl;
    cout << WHITE << "Total Workouts: " << RESET << BOLD << totalWorkouts << RESET << endl;
    cout << WHITE << "Total Duration: " << RESET << BOLD << totalDuration << RESET << " minutes" << endl;
    cout << endl;
    
    // Display workout history
    cout << BOLD << YELLOW << "📋 WORKOUT HISTORY" << RESET << endl;
    cout << BOLD << BLUE << string(40, '=') << RESET << endl;
    cout << endl;
    
    // Loop: display records in reverse order (newest first)
    for (int i = allUsers[currentUser].progressData.size() - 1; i >= 0; --i) {
        const ProgressRecord& record = allUsers[currentUser].progressData[i];
        
        cout << CYAN << "📅 " << record.date << " - " << record.day << RESET << endl;
        cout << WHITE << "🏋️  Exercises: " << RESET << record.exercises << endl;
        cout << WHITE << "⏱️  Duration: " << RESET << record.duration << " minutes" << endl;
        
    
        if (!record.notes.empty()) {
            cout << WHITE << "📝 Notes: " << RESET << record.notes << endl;
        }
        
        cout << MAGENTA << string(40, '-') << RESET << endl;
    }
    
    pauseScreen();
}

void exitProgram() {
    saveAllUsersData();
    
    cout << endl;
    cout << BOLD << GREEN;
    cout << "╔══════════════════════════════════════════════════╗" << endl;
    cout << "║  Thank you for using Workout Schedule Planner!   ║" << endl;
    cout << "║              Stay fit and healthy!               ║" << endl;
    cout << "╚══════════════════════════════════════════════════╝" << RESET << endl;
    cout << endl;
}
