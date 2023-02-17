import gym
import numpy as np
import time
class QLearningAgent(object):
    def __init__(self, stateSpace_size, actionSpace_size, learning_rate=0.01, gamma=0.9, epsilon=0.1):
        self.stateSpace_size = stateSpace_size
        self.actionSpace_size = actionSpace_size
        self.learning_rate = learning_rate
        self.gamma = gamma
        self.epsilon = epsilon
        self.QTable = np.zeros((stateSpace_size, actionSpace_size))
        self.trainTimes = 100
        self.runTimes = 10
    def setTrainTimes(self, train_times):
        self.trainTimes = train_times
    def setRunTimes(self, run_times):
        self.runTimes = run_times
    def setTrust(self, trust_ratio):
        self.epsilon = trust_ratio
    def getAction(self, state):
        sand = np.random.uniform(0, 1)
        if sand <= self.epsilon:
            action = np.random.choice(self.actionSpace_size)
        else:
            action = self.chooseActionFromQTable(state)
        return action
    def chooseActionFromQTable(self, state):
        Q_max = np.max(self.QTable[state, :])
        action_list = np.where(self.QTable[state, :] == Q_max)[0]
        action = np.random.choice(action_list)
        return action
    def updateQTable(self, state, action, reward, next_state, done):
        if done:
            Q_future = 0
        else:
            Q_future = np.max(self.QTable[next_state, :])
        updateValue = self.learning_rate * (reward + self.gamma * Q_future - self.QTable[state, action])
        self.QTable[state, action] += updateValue
    def showQTable(self):
        print(self.QTable)
    def loadQTable(self, path):
        self.QTable = np.load(path)
        print(path + ' loaded.')
    def saveQTable(self, path):
        np.save(path, self.QTable)
        print(path + ' saved.')
    def train(self, env, agent, render):
        total_steps = 0
        while True:
            state = env.reset()
            episode_reward = 0
            episode_steps = 0
            if total_steps >= self.trainTimes:
                break
            while True:
                action = self.getAction(state)
                next_state, reward, done, info = env.step(action)
                agent.updateQTable(state, action, reward, next_state, done)
                state = next_state
                episode_reward += reward
                episode_steps += 1
                if render:
                    env.render()
                if done:
                    total_steps += 1
                    print("episode "+str(total_steps)+", train steps "+str(episode_steps)+ ", reward "+str(
                        episode_reward))
                    break
    def run(self, env, agent, render):
        total_steps = 0
        while True:
            state = env.reset()
            episode_reward = 0
            episode_steps = 0
            if total_steps >= self.runTimes:
                break
            while True:
                action = self.chooseActionFromQTable(state)
                next_state, reward, done, info = env.step(action)
                state = next_state
                episode_reward += reward
                episode_steps += 1
                if render:
                    time.sleep(0.5)
                    env.render()
                if done:
                    total_steps += 1
                    print("episode " + str(total_steps) + ", run steps " + str(episode_steps) + ", reward " + str(
                        episode_reward))
                    break
render = True
env = gym.make('FrozenLake8x8-v1', is_slippery=False)
agent = QLearningAgent(
        stateSpace_size=env.observation_space.n,
        actionSpace_size=env.action_space.n,
        learning_rate=0.1,
        gamma=0.9,
        epsilon=0.3)
agent.loadQTable('./q_table.npy')
agent.setTrainTimes(1500)
agent.train(env, agent, False)
agent.showQTable()
agent.setRunTimes(1)
agent.run(env, agent, render)
agent.saveQTable('./q_table.npy')