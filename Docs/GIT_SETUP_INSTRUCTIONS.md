# Git Initialization and Branch Setup Instructions

Follow these steps to initialize your local repository, fetch branches, and create a new branch based on `develop`:

## 1. Initialize Your Local Repository

If you haven't already, initialize your repository:

```sh
git init
```

If you see `Reinitialized existing Git repository`, your repo is already initialized.

## 2. Set the Remote URL

Set the remote URL to the correct GitHub repository:

```sh
git remote set-url origin https://github.com/Ach57/COMP-345-Project.git
```

If you get `remote origin already exists`, skip `git remote add origin` and use the command above.

## 3. Fetch All Remote Branches

Fetch all branches from the remote:

```sh
git fetch --all
git branch -r
```

## 4. Create a New Branch Based on 'develop'

To create a new branch based on the remote `develop` branch:

1. Make sure you have fetched all remote branches (see above).
2. Check out the `develop` branch:
   ```sh
   git checkout develop
   ```
3. Create and switch to your new branch (replace `your-branch-name` with your desired branch name):
   ```sh
   git checkout -b your-branch-name
   ```
   This will create a new branch based on the latest state of `develop`.

---

**Contact your repository owner if you need access or further help.**

Follow these steps to initialize your local repository and connect it to the remote GitHub repository:

## 5. Creating a New Branch Based on 'develop'

To create a new branch based on the remote `develop` branch:

1. Make sure you have fetched all remote branches:
   ```sh
   git fetch --all
   ```
2. Check out the `develop` branch:
   ```sh
   git checkout develop
   ```
3. Create and switch to your new branch (replace `your-branch-name` with your desired branch name):
   ```sh
   git checkout -b your-branch-name
   ```
