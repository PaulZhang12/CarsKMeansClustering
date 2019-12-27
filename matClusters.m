data = dlmread('cars_clustered.csv', ',', 1, 0);
centroids = dlmread('clusters.csv', ',', 1, 0);

vector1 = data(:, 1);
vector2 = data(:, 2);
vector3 = data(:, 3);

colors = zeros(length(data),3);

for i = 1:length(data)
    if data(i, 8) == 1
        colors(i, :) = [1 0 0];
    elseif data(i, 8) == 2
        colors(i, :) = [0 1 0];
    else 
        colors(i, :) = [0 0 1];
    end
end

centroid1 = centroids(1:3, 1);
centroid2 = centroids(1:3, 2);
centroid3 = centroids(1:3, 3);

scatter3(vector1, vector2, vector3, 50, colors);
hold on
scatter3(centroid1, centroid2, centroid3, 500, 'k', 'X');
xlabel('mpg');
ylabel('cylinders');
zlabel('cubicinches');
hold off

