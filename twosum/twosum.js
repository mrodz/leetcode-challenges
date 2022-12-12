/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number[]}
 */
var twoSum = function (nums, target) {
	let required = {}

	for (let i = 0; i < nums.length; i++) {
		let diff = target - nums[i]

		if (nums[i] in required) {
			return [required[nums[i]], i]
		}

		required[diff] = i
	}
}