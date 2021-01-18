
import matplotlib.pyplot as plt
import os as os

SEQ_TIME = 158.748
THREADS = [1, 2, 4, 8]

# Very long lines times
VLL_PTH = [159.375, 88.977, 54.085, 45.570]
VLL_OMP = [160.993, 82.255, 49.732, 34.342]
VLL_MPI = [409.452, 180.397, 70.754, 40.335]
VLL_HYB = [389.589, 170.802, 67.043, 41.645]

# PROJECT_ROOT_DIR = "."
# CHAPTER_ID = "classification"
# IMAGES_PATH = os.path.join(PROJECT_ROOT_DIR, "graphs", CHAPTER_ID)
# os.makedirs(IMAGES_PATH, exist_ok = True)

# def save_fig(fig_id, tight_layout=True, fig_extension="png", resolution=300):
#     path = os.path.join(IMAGES_PATH, fig_id + "." + fig_extension)
#     print("Saving figure", fig_id)

#     if tight_layout:
#         plt.tight_layout()

#     plt.savefig(path, format=fig_extension, dpi=resolution)

print("======== PTHREADS ========\n")
threads = 1
idx = 0
for time in VLL_PTH:
    speedup = "{:.3f}".format(SEQ_TIME / time)
    efficiency = "{:.3f}".format(SEQ_TIME / time / THREADS[idx])

    print(f'{threads} threads: {efficiency} \n')
    threads = threads * 2
    idx = idx + 1

print("======== OMP ========\n")
threads = 1
idx = 0
for time in VLL_OMP:
    speedup = "{:.3f}".format(SEQ_TIME / time)
    efficiency = "{:.3f}".format(SEQ_TIME / time / THREADS[idx])

    print(f'{threads} threads: {efficiency} \n')
    threads = threads * 2
    idx = idx + 1

print("======== MPI ========\n")
threads = 1
idx = 0
for time in VLL_MPI:
    speedup = "{:.3f}".format(SEQ_TIME / time)
    efficiency = "{:.3f}".format(SEQ_TIME / time / THREADS[idx])

    print(f'{threads} threads: {efficiency} \n')
    threads = threads * 2
    idx = idx + 1

print("======== HYB ========\n")
threads = 1
idx = 0
for time in VLL_HYB:
    speedup = "{:.3f}".format(SEQ_TIME / time)
    efficiency = "{:.3f}".format(SEQ_TIME / time / THREADS[idx])

    print(f'{threads} threads: {efficiency} \n')
    threads = threads * 2
    idx = idx + 1