#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <random>
#include <cstdio>

void print_grid(const std::vector<std::vector<int>>& grid)
{
    for (int i = 0; i < grid[0].size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int print_clusters(const std::map<int, int>& clusters)
{
    int tot_cluster_size = 0;
    for (const auto& kv : clusters)
    {
        if (kv.second > 0)
        {
            tot_cluster_size += kv.second;
        }
    }
    return tot_cluster_size;
}

void hoshen_kopelman(std::vector<std::vector<int>>& grid, std::map<int, int>& clusters, int n, int m)
{
    int k = 2;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] > 0)
            {
                int left = 0, top = 0;
                if (j - 1 >= 0)
                    left = grid[i][j - 1];
                if (i - 1 >= 0)
                    top = grid[i - 1][j];

                if (left == 0 && top == 0)
                {
                    clusters[k] = 1;
                    grid[i][j] = k;
                    ++k;
                }
                else if ( (left > 0 && top == 0) || (left == top) && left > 0 || (left == 0 && top > 0))
                {
                    int clusterId = left > top ? left : top;
                    while (clusters[clusterId] < 0)
                        clusterId = -clusters[clusterId];
                    ++clusters[clusterId];
                    grid[i][j] = clusterId;
                    
                }
                else //
                {
                    int id_root_left = left;
                    int id_root_top = top;
                    while (clusters[id_root_left] < 0)
                        id_root_left = -clusters[id_root_left];

                    while (clusters[id_root_top] < 0)
                        id_root_top = -clusters[id_root_top];


                    if (clusters[id_root_left] > clusters[id_root_top])
                    {
                        clusters[id_root_left] += clusters[id_root_top] + 1;
                        clusters[id_root_top] = -id_root_left;
                        grid[i][j] = id_root_left;
                    }
                    else
                    {
                        if (id_root_left == id_root_top)
                        {
                            clusters[id_root_top]++;
                            grid[i][j] = id_root_top;
                        }
                        
                        else
                        {
                            clusters[id_root_top] += clusters[id_root_left] + 1;
                            clusters[id_root_left] = -id_root_top;
                            grid[i][j] = id_root_top;
                        }
                        
                    }
                }
            }
            
        }
    }
}

int fill_grid(std::vector<std::vector<int>>& grid, double p)
{
    std::mt19937 mt(1730);
    std::uniform_real_distribution<double> dist(0, 1);
    int occupied_sites = 0;
    for (int i = 0; i < (int)grid[0].size(); i++)
    {
        for (int j = 0; j < (int)grid.size(); j++)
        {
            if (dist(mt) < p)
            {
                grid[i][j] = 1;
                ++occupied_sites;
            }
        }
    }
    std::cout << "Occupied " << occupied_sites << " sites\n";
    return occupied_sites;
}



int main(int argc, char** argv)
{
    int dim = 10;
    if (argc == 2)
        dim = atoi(argv[1]);
    std::vector<std::vector<int>> grid(dim, std::vector<int>(dim));
    int occupied_sites = fill_grid(grid, 0.6);
    std::map<int, int> clusters{};
    hoshen_kopelman(grid, clusters, grid[0].size(), grid.size());
    int found_clusters = print_clusters(clusters);
    std::cout <<"Occupied sites: " << occupied_sites << "\tTotal sites found in clusters: " << found_clusters << "\n";

    std::getchar();
}