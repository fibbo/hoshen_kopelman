#include <vector>
#include <iostream>
#include <memory>
#include <random>

class Cluster
{
public:
    Cluster(int k) : id(k), size(1) {}
    int id = 0;
    int size = 0;
    std::shared_ptr<Cluster> parent = nullptr;
    int getSize()
    {
        if (parent != nullptr && *parent != *this)
        {
            return parent->getSize();
        }
        return size;
    }

    bool operator==(const Cluster& rhs)
    {
        return this->id == rhs.id;
    }

    bool operator!=(const Cluster& rhs)
    {
        return !(*this == rhs);
    }
};

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

void print_clusters(const std::vector<std::shared_ptr<Cluster>>& clusters)
{
    for (const auto& x : clusters)
    {
        if (x && x->parent == nullptr)
            std::cout << "cluster label: " << x->id << "  cluster size: " << x->getSize() << "\n";
    }
}

void print_clusters(const std::vector<int>& clusters)
{
    int tot_cluster_size = 0;
    for (int i = 0; i < clusters.size(); i++)
    {
        if (clusters[i] > 0)
            tot_cluster_size += clusters[i];
        std::cout << "cluster label: " << i << "  cluster size: " << clusters[i] << "\n";
    }
    std::cout << "total number of sites in all clusters: " << tot_cluster_size << "\n";
}

void hoshen_kopelman2(std::vector<std::vector<int>>& grid, std::vector<int>& clusters, int n, int m)
{
    int k = 2;
    for (int i = 0; i < n; ++i)
    {
        //print_grid(grid);
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] > 0)
            {
                int left = 0, top = 0;
                if (j - 1 >= 0)
                    left = grid[i][j - 1];
                if (i - 1 >= 0)
                    top = grid[i - 1][j];
                //std::cout << "[" << i << "][" << j << "]: "  "left: " << left << "\t top: " << top << "\n";

                if (left == 0 && top == 0)
                {
                    clusters[k] = 1;
                    grid[i][j] = k;
                    ++k;
                }
                else if ( (left > 0 && top == 0) || (left == top) || (left == 0 && top > 0))
                {
                    int clusterId = left > top ? left : top;
                    if (clusterId == 1)
                    {
                        clusters[k] = 1;
                        grid[i][j] = k;
                        k++;
                    }
                    else
                    {
                        ++clusters[clusterId];
                        grid[i][j] = clusterId;
                    }
                    
                }
                else //
                {
                    if (clusters[left] > clusters[top])
                    {
                        clusters[left] += clusters[top] + 1;
                        clusters[top] = -left;
                        grid[i][j] = left;
                        grid[i - 1][j] = left;
                    }
                    else
                    {
                        clusters[top] += clusters[left] + 1;
                        clusters[left] = -top;
                        grid[i][j] = top;
                        grid[i][j - 1] = top;
                    }
                }
            }
            
        }
        //print_clusters(clusters);
    }
}

void hoshen_kopelman(std::vector<std::vector<int>>& grid, std::vector<std::shared_ptr<Cluster>>& clusters, int n, int m)
{
    int k = 2;
    for (int i = 0; i < n; ++i)
    {
        print_grid(grid);
        print_clusters(clusters);
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] > 0)
            {
                int left = 0, top = 0;
                if (j-1 >= 0)
                    left = grid[i][j-1];
                if (i-1 >= 0)
                    top = grid[i-1][j];
                std::cout << "[" << i << "][" << j << "]: "  "left: " << left << "\t top: " << top << "\n";
                
                if (left == 0 && top == 0)
                {
                    clusters[k] = std::make_shared<Cluster>(Cluster(k));
                    grid[i][j] = k++;
                }
                else if (left > 0 && top == 0)
                {
                    auto cluster = clusters[left];
                    cluster->size++;
                    grid[i][j] = left;
                }
                else if (left == 0 && top > 0)
                {
                    auto cluster = clusters[top];
                    cluster->size++;
                    grid[i][j] = top;
                }
                else if (left > 0 && top > 0)
                {
                    auto top_cluster = clusters[top];
                    auto left_cluster = clusters[left];
                    if (top_cluster == left_cluster)
                    {
                        grid[i][j] = left;
                        grid[i - 1][j] = left;
                        ++left_cluster->size;
                    }
                    if (clusters[left]->size > clusters[top]->size)
                    {
                        left_cluster->size += top_cluster->getSize() + 1; 
                        clusters[top] = left_cluster;
                        grid[i-1][j] = left;
                        grid[i][j] = left;
                    }
                    else if (clusters[left]->size < clusters[top]->size)
                    {
                        top_cluster->size += left_cluster->getSize() + 1;
                        clusters[left] = top_cluster;
                        grid[i][j-1] = top;
                        grid[i][j] = top;
                    }

                }

                
            }
            
        }
        /* code */
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



int main()
{
    //std::vector<std::vector<int>> grid = { {1, 0, 0, 1, 0, 0}, 
    //                                       {0, 1, 1, 1, 1, 0},
    //                                       {0, 0, 1, 0, 0, 0},
    //                                       {1, 1, 0, 1, 1, 0},
    //                                       {0, 0, 1, 1, 0, 1},
    //                                       {0, 0, 0, 0, 1, 1}};
    int dim = 100;
    std::vector<std::vector<int>> grid(dim, std::vector<int>(dim));
    int occ_sites = fill_grid(grid, 0.6);
    std::vector<int> clusters(dim*dim, 0);
    hoshen_kopelman2(grid, clusters, grid[0].size(), grid.size());
    //std::vector<std::shared_ptr<Cluster>> clusters(grid[0].size()*grid.size()+2);
    //hoshen_kopelman(grid, clusters, grid[0].size(), grid.size());
    print_grid(grid);
    print_clusters(clusters);
    std::cout << occ_sites << "\n";
    std::getchar();
}