#include "counters.h"

Counters counters;

Counters::Counters()
    : query_url("/tmp/query_url")
    , user_url("/tmp/user_url")
    , user_rank("/tmp/user_rank")
    , query_user("/tmp/query_user")
    , query_rank("/tmp/query_rank")
{
}

void Counters::clear()
{
    query_url.clear();
    user_url.clear();
    user_rank.clear();
    query_user.clear();
    query_rank.clear();
}

void Counters::save(size_t start_day, size_t finish_day)
{
    string postfix = "_" + std::to_string(start_day) + "_" + std::to_string(finish_day);
    query_url.save("/Users/annasepliaraskaia/Desktop/work/query_url" + postfix);
    user_url.save("/Users/annasepliaraskaia/Desktop/work/user_url" + postfix);
    user_rank.save("/Users/annasepliaraskaia/Desktop/work/user_rank" + postfix);
    query_user.save("/Users/annasepliaraskaia/Desktop/work/query_user" + postfix);
    query_rank.save("/Users/annasepliaraskaia/Desktop/work/query_rank" + postfix);
}

void calculate_counters(const DayData& data)
{
    for(const auto& item0 : data)
    {
        for (const auto& item1 : item0.second)
        {
            const Query& query = item1.second;
            counters.query_user.get(query.id, query.person, 1)[0] += 1;

            for (size_t i = 0; i <= query.type.size(); ++i)
            {
                if (query.type[i] == 2)
                {
                    counters.user_url.get(query.person, query.urls[i], 1)[0] += 1;
                    counters.query_rank.get(query.id, i, 1)[0] += 1;
                }
            }

            size_t maxIndex = 0;
            int maxElem = query.type[0];
            for (size_t i = 0; i < 10; ++i)
            {
                if (query.type[i] >= maxElem) {
                    maxIndex = i;
                    maxElem = query.type[i];
                }
            }
            if (maxElem != 2) continue;
            for (size_t i = 0; i <= maxIndex; ++i)
            {
                vector<double>& vec = counters.query_url.get(query.id, query.urls[i], 4);
                vec[0] += 1;
                if (query.type[i] == 2) {
                    vec[1] += 1;
                } else {
                    vec[2] += 1;
                }
                if (i == maxIndex)
                {
                    vec[3] += 1;
                }
            }

            for (size_t i = 0; i <= maxIndex; ++i)
            {
                vector<double>& vec = counters.user_rank.get(query.person, i, 4);
                vec[0] += 1;
                if (query.type[i] == 2) {
                    vec[1] += 1;
                } else {
                    vec[2] += 1;
                }
                if (i == maxIndex)
                {
                    vec[3] += 1;
                }
            }
        }
    }
}

void calculate_counters(const string& folder, size_t start_day, size_t finish_day)
{
    counters.clear();
    clock_t start, end;
    for(size_t i = start_day; i <= finish_day; ++i)
    {
        std::cout << "evaluating day " << i << std::endl;
        start = clock();
        string file = folder + std::to_string(i) + ".txt";
        DayData data = read_day(file);
        end = clock();
        std::cout << i << " reading: " << double(end - start) /  CLOCKS_PER_SEC << std::endl;
        start = clock();
        calculate_counters(data);
        end = clock();
        std::cout << i << " calculations: " << double(end - start) /  CLOCKS_PER_SEC << std::endl;
    }
    counters.save(start_day, finish_day);
}

