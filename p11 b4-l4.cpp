using namespace std;
// todo добавить таймер, за сколько секунд выполнилась операция
#include <iostream>
#include <string>

#define stringToInteger stoi

const char DIVIDE_OP = ':';
const char ADD_OP = '*';
const char PLUS_OP = '+';
const char MINUS_OP = '-';
const char GREATER_OP = '>';
const char LOWER_OP = '<';
const char POW_OP = '^';
int PRECISE_GLOBAL = 8;
bool RESULT_WITH_SPACEBARS = false;
bool DEBUG = false;

int characterToInteger(char letter) {
    return letter - '0';
}

int maxFrom(string first, string second) {
    int maxLength;

    if (first.length() > second.length()) {
        maxLength = first.length();
    } else {
        maxLength = second.length();
    }

    return maxLength;
}

void reverseTwoNumber(string *first, string *second) {
    reverse(first->begin(), first->end());
    reverse(second->begin(), second->end());
}

string plusOperation(string firstNum, string secondNum) {
    int maxLength = 0;
    reverseTwoNumber(&firstNum, &secondNum);
    string output = "";
    
    maxLength = maxFrom(firstNum, secondNum);

    for (int letterIndex = 0; letterIndex < maxLength; ++letterIndex) {
        int firstDigit;
        if (letterIndex >= firstNum.length()) {
            firstDigit = 0;
        } else {
            firstDigit = characterToInteger(firstNum[letterIndex]);
        }

        int secondDigit; 
        if (letterIndex >= secondNum.length()) {
            secondDigit = 0;
        } else {
            secondDigit = characterToInteger(secondNum[letterIndex]);
        }

        int sum = firstDigit + secondDigit;

        if (output.length() - 1 == letterIndex) {
            int bonus = characterToInteger(output[0]);
            sum += bonus;
        }

        int nextSector = sum / 10;
        int thisSector = sum % 10;

        if (output.length() - 1 == letterIndex ) {
            output[0] = to_string(thisSector)[0];
        } else {
            output = to_string(thisSector) + output;
        }
        if (nextSector != 0) {
            output = to_string(nextSector) + output;
        }
    }

    return output;
}

void minusOneFromNextSector(string *originalNum, int letterIndex) {
    int newInd = letterIndex + 1;
    string copyNum = *originalNum;
    int newDigit = characterToInteger(copyNum[newInd]);
    int difference = newDigit - 1;

    if (difference < 0) {
        copyNum[newInd] = '9';
        *originalNum = copyNum; // +*originalNum;
        minusOneFromNextSector(originalNum, newInd);
    } else {
        copyNum[newInd] = to_string(difference)[0];
        *originalNum = copyNum;
    }
}

bool biggerThen(string first, string second) {
    if (first == second) {
        return false;
    }

    if (first.length() > second.length()) {
        return true;
    } else if (first.length() < second.length()) {
        return false;
    } else {
        for (int letterIndex = 0; letterIndex < first.length(); ++letterIndex) {
            int firstDigit = characterToInteger(first[letterIndex]);
            int secondDigit = characterToInteger(second[letterIndex]);

            if (firstDigit > secondDigit) {
                return true;
            } else if (firstDigit < secondDigit) {
                return false;
            }
        }
    }
}

void deleteFirstZeros(string *output) {
    string copy = *output;
    int letterIndex;

    if (output->length() == 1 && copy[0] == '0') {
        return;
    }

    for (letterIndex = 0; letterIndex < copy.length(); ++letterIndex) {
        if (copy[letterIndex] == '0') {
            continue;
        } else {
            break;
        }
    }

    *output = copy.substr(letterIndex);
}

void swapTwoValues(string *first, string *second) {
    string temp = *first;
    *first = *second;
    *second = temp;
}

string minusOperation(string firstNum, string secondNum) {
    bool minus = false;
    string output;
    
    if (!biggerThen(firstNum, secondNum) && firstNum != secondNum) {
        swapTwoValues(&firstNum, &secondNum);
        minus = true;
    }
    
    reverseTwoNumber(&firstNum, &secondNum);

    for (int letterIndex = 0; letterIndex < firstNum.length(); ++letterIndex) {
        int firstDigit;
        int secondDigit;
        
        // получение второй цифры
        if (letterIndex >= secondNum.length()) {
            secondDigit = 0;
        } else {
            secondDigit = characterToInteger(secondNum[letterIndex]);
        }

        // получение первой цифры
        if (letterIndex < output.length()) {
            firstDigit = characterToInteger(output[letterIndex]);
        } else {
            firstDigit = characterToInteger(firstNum[letterIndex]);
        }

        int diff = firstDigit - secondDigit;

        if (diff < 0) {
            // взятие единицы из следующего разряда, рекурсия
            diff = 10 + diff;
            minusOneFromNextSector(&firstNum, letterIndex);
        }
        // добавление результата к строке
        output = to_string(diff) + output;
    }

    deleteFirstZeros(&output);

    if (minus) {
        output = '-' + output;
    }

    return output;
}

string getZeros(int number) {
    string output = "";
    for (int letterIndex = 0; letterIndex < number; ++letterIndex) {
        output += "0";
    }

    return output;
}

string addOperation(string firstNum, string secondNum, bool debug=false) {
    string sum = "0";

    reverse(firstNum.begin(), firstNum.end());
    reverse(secondNum.begin(), secondNum.end());
    
    for (int firstInd = 0; firstInd < firstNum.length(); ++firstInd) {
        for (int secondInd = 0; secondInd < secondNum.length(); ++secondInd) {
            int firstDigit = characterToInteger(firstNum[firstInd]);
            int secondDigit = characterToInteger(secondNum[secondInd]);
            
            string result = to_string(firstDigit * secondDigit) + getZeros(firstInd) + getZeros(secondInd);

            sum = plusOperation(sum, result);

            if (debug) {
                cout << firstDigit << " * " << secondDigit << " = " << sum << "\n";
            }
        }
    }

    return sum;
}



string raiseSubNumberFromOriginalSource(int* start, string originalNumber, string subNumber) {
    string sub = originalNumber.substr(*start, 1);
    
    subNumber += sub;
    ++*start;
    

    return subNumber;
}

string getNod(string subNumber, string secondNumber) {
    if (subNumber == "0") {
        return "0";
    }

    bool isCycleEnded = false;
    int counter = 1;
    while (!isCycleEnded) {
        ++counter;
        string mostNearestNumber = addOperation(secondNumber, to_string(counter));
        if (biggerThen(mostNearestNumber, subNumber)) {
            counter -= 1;
            isCycleEnded = true;
            return to_string(counter);
        }
    }
}

string raiseLowerValue(string firstNum, string secondNum, int* preciseCount, string* output) {
    bool firstAdd = true;
    while (!biggerThen(firstNum, secondNum) && firstNum != secondNum) {
        if (firstAdd) {
            if (output->find(',') == string::npos) {
                if (output->empty()) {
                    *output = "0";
                }
                *output += ",";
            }
            firstNum += '0';
            firstAdd = false;
        } else {
            firstNum += '0';
            *output += "0";
        }

        --*preciseCount;

        if (DEBUG) {
            cout << "Первое число меньше второго, оно было увеличено в 10 раз. Результат: " + *output + ". Первое число: " + firstNum + "\n";
        }
    }

    return firstNum;
}

string divideOperation(string firstNumber, string secondNumber) {
    // todo деление игнорирует нули, когда те должны оставаться в оутпуте

    string output = "";

    int firstLen = firstNumber.length();
    int secondLen = secondNumber.length();

    // условия конца
    bool cycleEnded = false;
    int precision = PRECISE_GLOBAL;

    int start = 0;
    int end = 0;
    string lastPart = "0";

    firstNumber = raiseLowerValue(firstNumber, secondNumber, &precision, &output);

    while (!cycleEnded && precision > 0) {
        string subNumber;
        
        // получение подчисла
        if (start == 0 || lastPart == "0") {
            end = start + secondLen;
            subNumber = firstNumber.substr(start, secondLen);
            start += secondLen;
        } else if (lastPart == "0") {
            int end = start + secondLen;
            subNumber = firstNumber.substr(start, end);
            start = end;
        } else {
            subNumber = lastPart;
        }

        bool firstAdd = true;
        while (!biggerThen(subNumber, secondNumber) && subNumber != secondNumber && subNumber != "0") {
            if (start >= firstLen) {
                subNumber = raiseLowerValue(subNumber, secondNumber, &precision, &output);
            } else {
                subNumber = raiseSubNumberFromOriginalSource(&start, firstNumber, subNumber);
                
                if (lastPart != "0" && !firstAdd) {
                    output += "0";
                }
                firstAdd = false;
            }
            
        }
        
        string nod = getNod(subNumber, secondNumber);
        string fullPart = addOperation(secondNumber, nod);
        lastPart = minusOperation(subNumber, fullPart);

        
        cycleEnded = start >= firstLen && lastPart == "0";
        

        output += nod;
        if (DEBUG) {
            cout << "Наибольший общий делитель: " + nod + "; Получаемый результат: " + output + "; Подчисло: " + subNumber +
                "; Остаток от вычитания подчисла и второго числа: " + lastPart + "\n";
        }
    }

    return output;
    
}

char chooseOperation() {
    char operation;
    bool isAllRight = false;
    cout << "Выберите операцию: | " << PLUS_OP << " | " << DIVIDE_OP << " | " << MINUS_OP << " | " 
            << ADD_OP << " | " << GREATER_OP << " | " << LOWER_OP << " | " << POW_OP << '\n';
    while(!isAllRight) {
        cin >> operation;
        if ((operation != DIVIDE_OP) && (operation != PLUS_OP) && (operation != MINUS_OP) && (operation != ADD_OP) &&
            (operation != GREATER_OP) && (operation != LOWER_OP) && (operation != POW_OP)) {
            cout << "Неправильная операция! Попробуйте снова\n";
        } else {
            isAllRight = true;
        }
    }

    return operation;
}

bool hasTheMinus(string *number) {
    string copy = *number;

    return copy[0] == '-';
}

void clearMinus(string *number) {
    *number = number->substr(1);
}

bool handleWithMinus(string* first, string* second, char* operand) {
    bool minus = false;
    bool firstMinus = hasTheMinus(first);
    bool secondMinus = hasTheMinus(second);

    if (firstMinus) {
        clearMinus(first);
    }
    if (secondMinus) {
        clearMinus(second);
    }
    
    if (*operand == ADD_OP || *operand == DIVIDE_OP) {
        return firstMinus != secondMinus;
    }
    
    if ((*operand == PLUS_OP && firstMinus && secondMinus) == true ||
        (*operand == MINUS_OP && firstMinus && !secondMinus) == true) {
        *operand = PLUS_OP;
        return true;
    }

    if ((*operand == MINUS_OP && !firstMinus && secondMinus) == true) {
        *operand = PLUS_OP;
        return false;
    }

    if ((*operand == PLUS_OP && firstMinus && !secondMinus) == true ||
        (*operand == MINUS_OP && firstMinus && secondMinus) == true) {
        *operand = MINUS_OP;
        swapTwoValues(first, second);
    }

    if ((*operand == PLUS_OP && !firstMinus && secondMinus)) {
        *operand = MINUS_OP;
    }

    if (!biggerThen(*first, *second) && *first != *second && *operand == MINUS_OP) {
        swapTwoValues(first, second);
        return true;
    }

    return false;
}

string inputValue(string header) {
    cout << "Введите " + header + ": \n";
    string inputLine;
    while (inputLine.empty()) {
        getline(cin, inputLine);
        for (int letterIndex = 0; letterIndex < inputLine.length(); ++letterIndex) {
            char letter = inputLine[letterIndex];

            if (letterIndex == 0 && letter == '-') {
                continue;
            }

            if (!isdigit(letter)) {
                cout << "Неверный формат числа! Попробуйте снова\n";
                inputLine = "";
                break;
            }
        }
    }
    return inputLine;
}

string makeOperation(string first, string second, char operand) {
    string result;
    
    switch (operand) {
        case DIVIDE_OP:
            result = divideOperation(first, second);
            break;
        case PLUS_OP:
            result = plusOperation(first, second);
            break;
        case MINUS_OP:
            result = minusOperation(first, second);
            break;
        case ADD_OP:
            result = addOperation(first, second, DEBUG);
            break;
        case GREATER_OP:
            result = to_string(biggerThen(first, second));
            break;
        case LOWER_OP:
            result = to_string(biggerThen(second, first));
            break;
        case POW_OP:
            result = first;
            for (int letterIndex = 0; letterIndex < stringToInteger(second); letterIndex++) {
                result = addOperation(result, first);
            }
    }

    return result;
}

void greetingsSettings() {
    string option;
    while (true) {
        cout << "Добро пожаловать в калькулятор больших чисел!\nКалькулятор умеет работать с отрицательными значениями, можно ввести минус для любого числа\n";
        cout << "Также калькулятор имеет \"защиту от дурака\"\n";
        cout << "Текущие настройки программы:\n1. Кол-во знаков после запятой: " + to_string(PRECISE_GLOBAL) +
            "\n2. Вывод результата с пробелами: " + to_string(RESULT_WITH_SPACEBARS) + 
            "\n3. Включить режим отладки: " + to_string(DEBUG) +
            "\n0. Начать работу с программой\n";
        option = inputValue("опцию для настройки");
        if(option == "1") {
            system("cls");
            PRECISE_GLOBAL = stringToInteger(inputValue("кол-во знаков после запятой"));
        } else if (option == "2") {
            system("cls");
            RESULT_WITH_SPACEBARS = stringToInteger(inputValue("вывод результата с пробелами (0 или 1)"));
        } else if (option == "3") {
            system("cls");
            DEBUG = stringToInteger(inputValue("режим отладчика (0 или 1)"));
        } else if (option == "0") {
            system("cls");
            return;
        }        
    }   
}

string pasteSpacebars(string number) {
    reverse(number.begin(), number.end());
    string output;

    for (int letterIndex = 0; letterIndex < number.length(); ++letterIndex) {
        if (letterIndex % 3 == 0) {
            output += " ";
        }
        output += number[letterIndex];
    }
    reverse(output.begin(), output.end());
    return output;
}

int main() {
    setlocale(LC_ALL, "Russian");

    string inputLine;

    string firstNumber;
    char operand;
    string secondNumber;
    bool isMinusInResult;

    int condition = 0;

    greetingsSettings();

    while (inputLine != "end.") {
        switch (condition) {
            case 0:
                firstNumber = inputValue("первое число");
                break;
            case 1:
                operand = chooseOperation();
                break;
            case 2:
                secondNumber = inputValue("второе число");
  
                isMinusInResult = handleWithMinus(&firstNumber, &secondNumber, &operand);
                string result = makeOperation(firstNumber, secondNumber, operand);
                if (RESULT_WITH_SPACEBARS && result.find(',') == string::npos) {
                    result = pasteSpacebars(result);
                }
                if (isMinusInResult) {
                    result = '-' + result;
                }
                cout << "Результат: " << result << "\n\n";
        }
        
        ++condition;
        if (condition > 2) {
            condition = 0;
        }
    }
}

