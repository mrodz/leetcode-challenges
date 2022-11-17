class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        looking_for = {}
        for i, n in enumerate(nums):
            if n in looking_for:
                return looking_for[n], i
            looking_for[target - n] = i