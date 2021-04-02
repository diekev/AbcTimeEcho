
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcGeom/All.h>

using namespace Alembic;
using namespace Alembic::AbcCoreAbstract;

static constexpr auto verbose_log = false;

int main(int argc, const char **argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " FILE\n";
    return 1;
  }

  Alembic::AbcCoreFactory::IFactory factory;
  factory.setPolicy(Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
  auto archive = factory.getArchive(argv[1]);

  if (!archive.valid()) {
    std::cerr << "Cannot open " << argv[1] << " for reading !\n";
    return 1;
  }

  const uint32_t num_time_sampling_ptrs = archive.getNumTimeSamplings();
  uint32_t num_valid_time_sampling = 0;

  chrono_t min_time =  std::numeric_limits<chrono_t>::max();
  chrono_t max_time = -std::numeric_limits<chrono_t>::max();
  chrono_t fps = std::numeric_limits<chrono_t>::max();

  for (uint32_t i = 0; i < num_time_sampling_ptrs; ++i) {
    const TimeSamplingPtr ts = archive.getTimeSampling(i);

    if (verbose_log) {
      std::cout << ts->getTimeSamplingType() << '\n';
    }

    /* Ignore invalid or default TimeSampling. */
    if (ts->getNumStoredTimes() == 1 && ts->getStoredTimes()[0] == 0.0 && ts->getTimeSamplingType().getTimePerCycle() == 1) {
      continue;
    }

    const index_t max_samples = archive.getMaxNumSamplesForTimeSamplingIndex(i);

    if (max_samples == INDEX_UNKNOWN) {
      continue;
    }

    min_time = std::min(min_time, ts->getSampleTime(0));
    max_time = std::max(max_time, ts->getSampleTime(max_samples - 1));

    const auto &time_sampling_type = ts->getTimeSamplingType();
    const auto time_per_cycle = time_sampling_type.getTimePerCycle();

    if (time_per_cycle == 0.0) {
      continue;
    }

    if (time_sampling_type.isUniform()) {
      fps = 1.0 / time_per_cycle;
    }
    else {
      const auto num_samples_per_cycle = time_sampling_type.getNumSamplesPerCycle();

      if (num_samples_per_cycle == 0) {
        continue;
      }

      fps = 1.0 / (time_per_cycle / num_samples_per_cycle);
    }

    num_valid_time_sampling++;
  }

  std::cout << "Number of TimeSamplings in the file : " << num_time_sampling_ptrs << "\n";
  std::cout << "Number of valid TimeSamplings in the file : " << num_valid_time_sampling << "\n";
  std::cout << "\n";

  bool has_error = false;

  if (min_time == std::numeric_limits<chrono_t>::max() || min_time > max_time) {
    if (verbose_log) {
      std::cout << "Error: cannot determine min_time\n";
    }
    has_error = true;
  }

  if (max_time == -std::numeric_limits<chrono_t>::max() || max_time < min_time) {
    if (verbose_log) {
      std::cout << "Error: cannot determine max_time\n";
    }
    has_error = true;
  }

  if (fps == 0.0 || fps == std::numeric_limits<chrono_t>::max()) {
    if (verbose_log) {
      std::cout << "Error: cannot determine fps\n\n";
    }
    has_error = true;
  }

  if (has_error) {
    std::cout << "Error: cannot determine the time information from the archive !\n";
    return 1;
  }

  std::cout << "Minimum time in seconds : " << min_time << "\n";
  std::cout << "Maximum time in seconds : " << max_time << "\n";
  std::cout << "\n";
  std::cout << "Minimum frame : " << min_time * fps << "\n";
  std::cout << "Maximum frame : " << max_time * fps << "\n";
  std::cout << "\n";
  std::cout << "Frame rate : " << fps << " FPS\n";
  std::cout << "\n";

  return 0;
}
