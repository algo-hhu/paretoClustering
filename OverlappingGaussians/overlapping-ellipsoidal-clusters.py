import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_gaussian_quantiles
from scipy.stats import multivariate_t

# Set a seed for reproducibility
np.random.seed(41) 

# Function to create a Gaussian cluster
def create_gaussian_cluster(mean, cov, num_samples):
    """
    Create a Gaussian cluster.

    Parameters:
    - mean: The mean of the Gaussian cluster (x, y).
    - cov: The covariance matrix defining the shape of the cluster.
    - num_samples: The number of samples to generate.

    Returns:
    - X: Array of points representing the Gaussian cluster.
    """
    X = np.random.multivariate_normal(mean, cov, num_samples)
    return X

def create_gaussian(mean,eigenvectors,eigenvalues,num_samples):

    normalized_eigenvectors = eigenvectors / np.linalg.norm(eigenvectors, axis=1, keepdims=True)
    
    # Constructing the Lambda and Q matrices
    Lambda = np.diag(eigenvalues)
    Q = normalized_eigenvectors

    # Constructing the covariance matrix
    covariance_matrix = Q.dot(Lambda).dot(Q.T)

    X = create_gaussian_cluster(mean,covariance_matrix,num_samples)
    return X


def generate_s_cluster(center=(0, 0), length=10, points=100, noise_scale=0.1, rotation_angle=0):
    """
    Generates points for a rotated cluster in the shape of an "S".

    Parameters:
    - center: A tuple (x, y) representing the center of the "S".
    - length: The length of the "S" curve.
    - points: The number of points to generate along the "S".
    - noise_scale: The standard deviation of the Gaussian noise applied to the points.
    - rotation_angle: The angle to rotate the cluster, in degrees.

    Returns:
    - A NumPy array of shape (points, 2), where each row is a point (x, y).
    """

    # Generate the S curve
    x = np.linspace(-length / 2, length / 2, points)
    y = np.sin(x) * length / 2

    # Center adjustment before rotation
    x_center_adjusted = x
    y_center_adjusted = y

    # Apply rotation
    theta = np.radians(rotation_angle)  # Convert angle to radians
    cos_theta, sin_theta = np.cos(theta), np.sin(theta)
    x_rotated = cos_theta * x_center_adjusted - sin_theta * y_center_adjusted
    y_rotated = sin_theta * x_center_adjusted + cos_theta * y_center_adjusted

    # Adjust for the specified center
    x_rotated += center[0]
    y_rotated += center[1]

    # Apply Gaussian noise
    x_noise = np.random.normal(0, noise_scale, points)
    y_noise = np.random.normal(0, noise_scale, points)
    x_noisy = x_rotated + x_noise
    y_noisy = y_rotated + y_noise

    # Return the points as a 2D NumPy array
    return np.column_stack((x_noisy, y_noisy))


def generate_cross_cluster(center=(0, 0), horizontal_arm_length=10, vertical_arm_length=10,
                                points_per_arm=50, noise_scale=0.5, rotation_angle_deg=0):
    """
    Generates points for a noisy cross with specified parameters, rotates it by a given angle, 
    and returns them as an array.

    Parameters:
    - center: A tuple (x, y) representing the center of the cross.
    - horizontal_arm_length: The length of the horizontal arm of the cross.
    - vertical_arm_length: The length of the vertical arm of the cross.
    - points_per_arm: The number of points per arm.
    - noise_scale: The standard deviation of the Gaussian noise applied to the points.
    - rotation_angle_deg: The angle (in degrees) to rotate the cross.
    
    Returns:
    - A NumPy array of shape (n_points, 2) containing the x and y coordinates of the rotated points.
    """
    
    # Generate points for each arm
    x_horizontal = np.linspace(center[0] - horizontal_arm_length, center[0] + horizontal_arm_length, points_per_arm)
    y_horizontal = np.full(points_per_arm, center[1])
    y_vertical = np.linspace(center[1] - vertical_arm_length, center[1] + vertical_arm_length, points_per_arm)
    x_vertical = np.full(points_per_arm, center[0])

    # Generate Gaussian noise
    x_noise_horizontal = np.random.normal(0, noise_scale, points_per_arm)
    y_noise_horizontal = np.random.normal(0, noise_scale, points_per_arm)
    x_noise_vertical = np.random.normal(0, noise_scale, points_per_arm)
    y_noise_vertical = np.random.normal(0, noise_scale, points_per_arm)

    # Apply Gaussian noise to the points
    x_points_noisy = np.concatenate((x_horizontal + x_noise_horizontal, x_vertical + x_noise_vertical))
    y_points_noisy = np.concatenate((y_horizontal + y_noise_horizontal, y_vertical + y_noise_vertical))

    # Combine x and y coordinates into a single array
    points_array = np.vstack((x_points_noisy, y_points_noisy)).T
    
    # Rotation matrix
    theta = np.radians(rotation_angle_deg)
    rotation_matrix = np.array([[np.cos(theta), -np.sin(theta)], [np.sin(theta), np.cos(theta)]])
    
    # Rotate points
    rotated_points = np.dot(points_array - np.array(center), rotation_matrix) + np.array(center)
    
    return rotated_points

eigenvectors = np.array([[1, 0], [0, 1]])  # Eigenvectors

cluster_1 = create_gaussian([-4, 7],np.array([[1, 1], [1,-1]]),[5,0.5],100)

cluster_2 = create_gaussian([13, 0],np.array([[1, 1], [-1,1]]),[0.5,5],1500)

cluster_3 = create_gaussian([4.5, 7.5],np.array([[-1, 1], [1,1]]),[8,0.5],1000)

cluster_s_shape = generate_s_cluster(center=(2, -14), length=10, points=300, noise_scale=0.4, rotation_angle=45)

cluster_cross = generate_cross_cluster(center=(-10, -4), horizontal_arm_length=10, vertical_arm_length=7,
                                     points_per_arm=250, noise_scale=0.5, rotation_angle_deg=45)



plt.scatter(cluster_1[:, 0], cluster_1[:, 1], s=5,alpha=0.6)
plt.scatter(cluster_2[:, 0], cluster_2[:, 1], s=5,alpha=0.6)
plt.scatter(cluster_3[:, 0], cluster_3[:, 1], s=5,alpha=0.6)
plt.scatter(cluster_s_shape[:,0], cluster_s_shape[:,1], s=5, alpha=0.6) 
plt.scatter(cluster_cross[:,0], cluster_cross[:,1], s=5, alpha=0.6) 


mean = [0, 0]  # Mean of the distribution
shape = [[1, 0], [0, 1]]  # Shape matrix, similar to the covariance matrix
df = 3  # Degrees of freedom, lower means heavier tails
size = 200  # Number of samples

# Generate samples
samples = multivariate_t.rvs(mean, shape, df, size=size)

plt.scatter(samples[:, 0], samples[:, 1],s=5, alpha=0.6)

def create_random_cluster(x_min, x_max, y_min, y_max, num_samples=100):
    """
    Create a random cluster distributed over a specified square.

    Parameters:
    - x_min: The minimum x-coordinate of the square.
    - x_max: The maximum x-coordinate of the square.
    - y_min: The minimum y-coordinate of the square.
    - y_max: The maximum y-coordinate of the square.
    - num_samples: The number of random samples to generate (default is 100).

    Returns:
    - X: Array of points representing the random cluster.
    """
    x_coords = np.random.uniform(x_min, x_max, num_samples)
    y_coords = np.random.uniform(y_min, y_max, num_samples)
    X = np.vstack((x_coords, y_coords)).T
    return X

random_cluster = create_random_cluster(x_min=-10, x_max=16, y_min=-10, y_max=10, num_samples=100)

plt.scatter(random_cluster[:, 0], random_cluster[:, 1], s=5,alpha=0.6, c='k')  # 'k' for black points

plt.title('')
plt.xlabel('')
plt.ylabel('')
plt.axis('equal')

plt.savefig('overlapping-gaussians_plot.jpg', format='jpg', dpi=150)

print("Plot has been written to overlapping-gaussians.jpg")

plt.show()

all_clusters = np.vstack((cluster_1, cluster_2, cluster_3, cluster_s_shape, cluster_cross, samples, random_cluster))
# Prepare a list of cluster indices for each point
cluster_indices = np.concatenate([
    np.zeros(len(cluster_1)),  # Index 0 for cluster_1
    np.ones(len(cluster_2)),  # Index 1 for cluster_2
    2*np.ones(len(cluster_3)),  # Index 2 for cluster_3
    3*np.ones(len(cluster_s_shape)),  # Index 3 for cluster_s_shape
    4*np.ones(len(cluster_cross)),  # Index 4 for cluster_cross
    5*np.ones(len(samples)),  # Index 5 for samples
    6*np.ones(len(random_cluster))  # Index 6 for random_cluster
]).astype(int)

with open('overlapping-gaussians.data', 'w') as file:
    for point in all_clusters:
        file.write(f"{point[0]} {point[1]}\n")


print("Dataset has been written to overlapping-gaussians.data")

with open('overlapping-gaussians-ground-truth.data', 'w') as file:
    for i, cluster_index in enumerate(cluster_indices):
        file.write(f"{i} {cluster_index}\n")

print("Ground truth has been written to overlapping-gaussians-ground-truth.data")

with open('overlapping-gaussians-combined.data', 'w') as file:
    for i, (point, cluster_index) in enumerate(zip(all_clusters, cluster_indices)):
        file.write(f"{point[0]} {point[1]} {cluster_index}\n")

print("Ground truth and points have been written to overlapping-gaussians-combined.data")