#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <format>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "improvements/utils/text/UTF8/UTF8.h"
#include "improvements/abstraction/efor/efor.h"
#include "improvements/abstraction/print/print.h"
#include "improvements/utils/text/Art/Ascii.h"
#include "improvements/utils/text/Color/color.h"
#include "improvements/utils/text/Cls/console.h"
#include "improvements/abstraction/java/StringBuilder/StringBuilder.h"
#include "improvements/utils/Additions/log/log.h"
#include "improvements/utils/Additions/Types/value.h"
#include "improvements/utils/Additions/random/random.h"
#include "improvements/utils/Additions/Time/ETimer.h"

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;
using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace std::chrono_literals;
using namespace std::literals;
using namespace ascii;
