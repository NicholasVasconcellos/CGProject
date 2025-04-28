import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os

# Set the style for the plot
plt.style.use('ggplot')
sns.set_palette("deep")

# Get all CSV files from the pointSets directory
point_sets_dir = '/mnt/c/Users/Nicholas/Developer/CGProject/results'
csv_files = [f for f in os.listdir(point_sets_dir) if f.endswith('.csv')]

if not csv_files:
    raise FileNotFoundError("No CSV files found in Directory.")

# Process each CSV file
for csv_file in csv_files:
    # Read the CSV file
    file_path = os.path.join(point_sets_dir, csv_file)
    data = pd.read_csv(file_path)
    
    # Sort data by ClusterSize for better visualization
    data = data.sort_values('ClusterSize')
    
    # Create figure and axis for this file
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Create the histogram using bar plot
    bars = ax.bar(data['ClusterSize'], data['Count'], color='steelblue', alpha=0.8, 
           width=0.8, edgecolor='black', linewidth=0.5)
    
    # Add data labels on top of each bar
    for bar in bars:
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height + 5,
                f'{height}', ha='center', va='bottom', fontsize=9)
    
    # Add a title and labels including the file name
    csv_filename = os.path.splitext(csv_file)[0]  # Get name without extension
    ax.set_title(f'Cluster Size Distribution: {csv_filename}', fontsize=16, pad=20)
    ax.set_xlabel('Cluster Size', fontsize=14, labelpad=10)
    ax.set_ylabel('Frequency (Count)', fontsize=14, labelpad=10)
    
    # Add grid lines for better readability
    ax.yaxis.grid(True, linestyle='--', alpha=0.7)
    
    # Set the x-axis to show all cluster sizes
    ax.set_xticks(data['ClusterSize'])
    
    # Tight layout to ensure everything fits
    plt.tight_layout()
    
    # Save the figure in the images folder with the same name as the original CSV
    images_dir = '/mnt/c/Users/Nicholas/Developer/CGProject/images'
    os.makedirs(images_dir, exist_ok=True)
    plt.savefig(os.path.join(images_dir, f'{csv_filename}.png'), dpi=300, bbox_inches='tight')
    
    # Close the figure to free memory (important when processing many files)
    plt.close(fig)

print(f"Created histograms for {len(csv_files)} CSV files.")