#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include <cstdio>

class Cluster;
using ClusterPtr = std::shared_ptr<Cluster>;
using ClusterWPtr = std::weak_ptr<Cluster>;

class Cluster : public std::enable_shared_from_this<Cluster>
{
public:
    Cluster(int k) : m_iId(k), m_iSize(1) {}
    int m_iId = 0;
    int m_iSize = 0;
    ClusterPtr m_pRoot = nullptr;
    int getSize()
    {
        if (m_pRoot != nullptr)
        {
            return m_pRoot->getSize();
        }
        return m_iSize;
    }

    void merge(ClusterPtr c)
    {
        if (this == c.get())
        {
            m_iSize++;
            return;
        }
        ClusterPtr x = c;
        while (x->m_pRoot)
        {
            x = x->m_pRoot;
            std::cout << "Merging the root\n";
        }
        m_iSize += x->getSize() + 1;
        x->m_iSize = 0;
        x->m_pRoot.reset();
        x->m_pRoot = shared_from_this();
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

int print_clusters(const std::vector<ClusterPtr>& clusters)
{
    int tot_cluster_size = 0;
    for (const auto& x : clusters)
    {
        
        if (x && x->m_pRoot == nullptr)
        {
            std::cout << "cluster label: " << x->m_iId << "  cluster size: " << x->getSize() << "\n";
            tot_cluster_size += x->getSize();
        }
    }
    return tot_cluster_size;
}

int print_clusters(const std::vector<int>& clusters)
{
    int tot_cluster_size = 0;
    for (int i = 0; i < clusters.size(); i++)
    {
        if (clusters[i] > 0)
            tot_cluster_size += clusters[i];
        std::cout << "cluster label: " << i << "  cluster size: " << clusters[i] << "\n";
    }
    return tot_cluster_size;
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

void hoshen_kopelman(std::vector<std::vector<int>>& grid, std::vector<ClusterPtr>& clusters, int n, int m)
{
    int k = 2;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] > 0)
            {
                int left = 0, top = 0;
                if (j-1 >= 0)
                    left = grid[i][j-1];
                if (i-1 >= 0)
                    top = grid[i-1][j];
                if (left == 0 && top == 0)
                {
                    clusters[k] = std::make_shared<Cluster>(Cluster(k));
                    grid[i][j] = k++;
                }
                else if (left > 0 && top == 0)
                {
                    auto cluster = clusters[left];
                    cluster->m_iSize++;
                    grid[i][j] = left;
                }
                else if (left == 0 && top > 0)
                {
                    auto cluster = clusters[top];
                    cluster->m_iSize++;
                    grid[i][j] = top;
                }
                else if (left > 0 && top > 0)
                {
                    auto top_cluster = clusters[top];
                    auto left_cluster = clusters[left];

                    if (clusters[left]->m_iSize > clusters[top]->m_iSize)
                    {
                        left_cluster->merge(top_cluster);
                        //clusters[top] = left_cluster;
                        grid[i-1][j] = left;
                        grid[i][j] = left;
                    }
                    else
                    {
                        top_cluster->merge(left_cluster);
                        //clusters[left] = top_cluster;
                        grid[i][j-1] = top;
                        grid[i][j] = top;
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
    //std::vector<std::vector<int>> grid = { {1, 0, 0, 1, 0, 0}, 
    //                                       {0, 1, 1, 1, 1, 0},
    //                                       {0, 0, 1, 0, 0, 0},
    //                                       {1, 0, 0, 1, 1, 0},
    //                                       {0, 1, 1, 1, 0, 1},
    //                                       {0, 0, 0, 0, 1, 1}};
    //int occ_sites = 15;
    int dim = 100;
    if (argc == 2)
        dim = atoi(argv[1]);
    std::vector<std::vector<int>> grid(dim, std::vector<int>(dim));
    int occ_sites = fill_grid(grid, 0.6);
    
    std::vector<ClusterPtr> clusters(grid[0].size()*grid.size()+2);
    hoshen_kopelman(grid, clusters, grid[0].size(), grid.size());
    print_grid(grid);
    int found_clusters = print_clusters(clusters);
    std::cout <<"Occupied sites: " << occ_sites << "Found in clusters: " << found_clusters << "\n";
}