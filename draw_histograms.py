import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os
from matplotlib import gridspec
from matplotlib.ticker import ScalarFormatter

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
    
    # Create evenly spaced positions for the bars
    x_positions = np.arange(len(data))
    
    # Get CSV filename without extension
    csv_filename = os.path.splitext(csv_file)[0]
    
    # 1. Create standard plot with evenly spaced bars
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Create the histogram using bar plot with evenly spaced positions
    bars = ax.bar(x_positions, data['Count'], color='steelblue', alpha=0.8, 
           width=0.8, edgecolor='black', linewidth=0.5)
    
    # Add data labels on top of each bar
    for i, bar in enumerate(bars):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height + (max(data['Count']) * 0.01), 
                f'{height}', ha='center', va='bottom', fontsize=9)
        
        # Add the actual cluster size below each bar
        ax.text(bar.get_x() + bar.get_width()/2., -max(data['Count']) * 0.03, 
                f'{data.iloc[i]["ClusterSize"]}', ha='center', va='top', 
                fontsize=9, rotation=45 if len(data) > 10 else 0)
    
    # Add a title and labels
    ax.set_title(f'Cluster Size Distribution: {csv_filename}', fontsize=16, pad=20)
    ax.set_xlabel('Cluster Size', fontsize=14, labelpad=10)
    ax.set_ylabel('Frequency (Count)', fontsize=14, labelpad=10)
    
    # Add grid lines for better readability
    ax.yaxis.grid(True, linestyle='--', alpha=0.7)
    
    # Set x-ticks to be evenly spaced but don't show labels (we've added custom labels)
    ax.set_xticks(x_positions)
    ax.set_xticklabels([])
    
    # Add some padding at the bottom for rotated labels
    plt.subplots_adjust(bottom=0.15, top=0.9, left=0.1, right=0.95)
    
    # Save the standard figure
    images_dir = '/mnt/c/Users/Nicholas/Developer/CGProject/images'
    os.makedirs(images_dir, exist_ok=True)
    plt.savefig(os.path.join(images_dir, f'{csv_filename}.png'), dpi=300, bbox_inches='tight')
    plt.close(fig)
    
    # 2. Create log scale version
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Create the histogram with log scale for y-axis
    bars = ax.bar(x_positions, data['Count'], color='steelblue', alpha=0.8, 
           width=0.8, edgecolor='black', linewidth=0.5)
    
    # Set y-axis to log scale, to better show the distribution
    ax.set_yscale('log')
    
    # Format y-axis to show actual numbers rather than powers of 10
    ax.yaxis.set_major_formatter(ScalarFormatter())
    
    # Add data labels on top of each bar
    for i, bar in enumerate(bars):
        height = bar.get_height()
        # Position labels at fixed offset in log scale
        label_y = height * 1.1
        ax.text(bar.get_x() + bar.get_width()/2., label_y, 
                f'{height}', ha='center', va='bottom', fontsize=9)
        
        # Add the actual cluster size below each bar
        ax.text(bar.get_x() + bar.get_width()/2., 0.5, 
                f'{data.iloc[i]["ClusterSize"]}', ha='center', va='top', 
                fontsize=9, rotation=45 if len(data) > 10 else 0)
    
    # Add a title and labels
    ax.set_title(f'Cluster Size Distribution (Log Scale): {csv_filename}', fontsize=16, pad=20)
    ax.set_xlabel('Cluster Size', fontsize=14, labelpad=10)
    ax.set_ylabel('Frequency (Count) - Log Scale', fontsize=14, labelpad=10)
    
    # Add grid lines for better readability
    ax.yaxis.grid(True, linestyle='--', alpha=0.7)
    
    # Set x-ticks to be evenly spaced but don't show labels
    ax.set_xticks(x_positions)
    ax.set_xticklabels([])
    
    # Ensure y-axis starts at 1 or lower for log scale
    ax.set_ylim(bottom=max(0.9, min(data['Count']) * 0.9))
    
    # Add some padding
    plt.subplots_adjust(bottom=0.15, top=0.9, left=0.1, right=0.95)
    
    # Save the log scale figure
    plt.savefig(os.path.join(images_dir, f'{csv_filename}_log.png'), dpi=300, bbox_inches='tight')
    plt.close(fig)

print(f"Created standard and log-scale histograms for {len(csv_files)} CSV files.")